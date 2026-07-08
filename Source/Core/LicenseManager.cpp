#include "LicenseManager.h"

LicenseManager::LicenseManager()
{
    info.machineCode = buildLocalMachineCode();
    info.firstRun = juce::Time::getCurrentTime();
    load();
    updateTrialState();
}

void LicenseManager::load()
{
    const auto file = getLicenseFile();

    if (! file.existsAsFile())
    {
        save();
        return;
    }

    std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (file));
    if (xml == nullptr || ! xml->hasTagName ("VocalSuiteUltraProLicense"))
    {
        info.status = Status::invalid;
        return;
    }

    info.customerName = xml->getStringAttribute ("name");
    info.customerEmail = xml->getStringAttribute ("email");
    info.licenseKey = xml->getStringAttribute ("key");
    info.machineCode = xml->getStringAttribute ("machine", buildLocalMachineCode());
    info.firstRun = juce::Time (xml->getStringAttribute ("firstRun").getLargeIntValue());
    info.activatedAt = juce::Time (xml->getStringAttribute ("activatedAt").getLargeIntValue());
    info.trialDays = xml->getIntAttribute ("trialDays", 14);
    info.offlineActivation = xml->getBoolAttribute ("offline", false);

    const auto statusText = xml->getStringAttribute ("status", "trial");
    if (statusText == "activated") info.status = Status::activated;
    else if (statusText == "expired") info.status = Status::expired;
    else if (statusText == "invalid") info.status = Status::invalid;
    else info.status = Status::trial;

    if (info.status == Status::activated)
    {
        const auto expected = makeOfflineKeyForMachine (info.customerName, info.customerEmail, info.machineCode);
        if (info.licenseKey != expected)
            info.status = Status::invalid;
    }
}

void LicenseManager::save() const
{
    auto file = getLicenseFile();
    file.getParentDirectory().createDirectory();

    juce::XmlElement xml ("VocalSuiteUltraProLicense");
    xml.setAttribute ("product", "Vocal Suite Ultra Pro");
    xml.setAttribute ("version", "5.3.0");
    xml.setAttribute ("status", statusToString (info.status));
    xml.setAttribute ("name", info.customerName);
    xml.setAttribute ("email", info.customerEmail);
    xml.setAttribute ("key", info.licenseKey);
    xml.setAttribute ("machine", info.machineCode);
    xml.setAttribute ("firstRun", juce::String (info.firstRun.toMilliseconds()));
    xml.setAttribute ("activatedAt", juce::String (info.activatedAt.toMilliseconds()));
    xml.setAttribute ("trialDays", info.trialDays);
    xml.setAttribute ("offline", info.offlineActivation);
    xml.writeTo (file);
}

void LicenseManager::resetTrial()
{
    info.status = Status::trial;
    info.firstRun = juce::Time::getCurrentTime();
    info.activatedAt = {};
    info.customerName.clear();
    info.customerEmail.clear();
    info.licenseKey.clear();
    info.offlineActivation = false;
    save();
}

bool LicenseManager::activateOffline (const juce::String& name,
                                      const juce::String& email,
                                      const juce::String& key)
{
    const auto cleanName = name.trim();
    const auto cleanEmail = email.trim().toLowerCase();
    const auto cleanKey = key.trim().toUpperCase();
    const auto expected = makeOfflineKeyForMachine (cleanName, cleanEmail, info.machineCode);

    if (cleanName.isEmpty() || cleanEmail.isEmpty() || cleanKey != expected)
    {
        info.status = Status::invalid;
        save();
        return false;
    }

    info.customerName = cleanName;
    info.customerEmail = cleanEmail;
    info.licenseKey = cleanKey;
    info.status = Status::activated;
    info.activatedAt = juce::Time::getCurrentTime();
    info.offlineActivation = true;
    save();
    return true;
}

void LicenseManager::deactivate()
{
    info.status = Status::trial;
    info.activatedAt = {};
    info.licenseKey.clear();
    info.offlineActivation = false;
    updateTrialState();
    save();
}

int LicenseManager::getTrialDaysRemaining() const
{
    if (info.status == Status::activated)
        return info.trialDays;

    const auto elapsedMs = juce::Time::getCurrentTime().toMilliseconds() - info.firstRun.toMilliseconds();
    const auto elapsedDays = (int) (elapsedMs / (1000LL * 60LL * 60LL * 24LL));
    return juce::jlimit (0, info.trialDays, info.trialDays - elapsedDays);
}

juce::String LicenseManager::getStatusText() const
{
    switch (info.status)
    {
        case Status::activated: return "Licensed to " + info.customerName;
        case Status::trial:     return "Trial " + juce::String (getTrialDaysRemaining()) + " days left";
        case Status::expired:   return "Trial expired";
        case Status::invalid:   return "License invalid";
    }

    return "License unknown";
}

juce::String LicenseManager::statusToString (Status status)
{
    switch (status)
    {
        case Status::activated: return "activated";
        case Status::trial:     return "trial";
        case Status::expired:   return "expired";
        case Status::invalid:   return "invalid";
    }

    return "invalid";
}

juce::String LicenseManager::makeOfflineKeyForMachine (const juce::String& name,
                                                       const juce::String& email,
                                                       const juce::String& machineCode)
{
    const auto seed = normalize (name) + "|" + normalize (email) + "|" + normalize (machineCode) + "|VSUP|5D";
    auto hex = makeHashText (seed).toUpperCase();

    while (hex.length() < 20)
        hex += makeHashText (hex + seed).toUpperCase();

    return "VSUP-" + hex.substring (0, 5) + "-" + hex.substring (5, 10) + "-" +
           hex.substring (10, 15) + "-" + hex.substring (15, 20);
}

juce::File LicenseManager::getLicenseFile() const
{
    return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
        .getChildFile ("Hai Pham Audio")
        .getChildFile ("Vocal Suite Ultra Pro")
        .getChildFile ("license.xml");
}

juce::String LicenseManager::buildLocalMachineCode() const
{
    const auto seed = juce::SystemStats::getComputerName() + "|" +
                      juce::SystemStats::getLogonName() + "|" +
                      juce::SystemStats::getOperatingSystemName();

    auto hex = makeHashText (seed).toUpperCase();
    while (hex.length() < 16)
        hex += makeHashText (hex + seed).toUpperCase();

    return hex.substring (0, 4) + "-" + hex.substring (4, 8) + "-" +
           hex.substring (8, 12) + "-" + hex.substring (12, 16);
}

void LicenseManager::updateTrialState()
{
    if (info.status == Status::activated || info.status == Status::invalid)
        return;

    info.status = getTrialDaysRemaining() > 0 ? Status::trial : Status::expired;
}

juce::String LicenseManager::normalize (juce::String text)
{
    return text.trim().toLowerCase().retainCharacters ("abcdefghijklmnopqrstuvwxyz0123456789@._-");
}

juce::String LicenseManager::makeHashText (const juce::String& text)
{
    auto a = (juce::int64) text.hashCode64();
    auto b = (juce::int64) (text + "Hai Pham Audio").hashCode64();
    auto hex = juce::String::toHexString (a) + juce::String::toHexString (b);
    return hex.retainCharacters ("0123456789abcdefABCDEF");
}
