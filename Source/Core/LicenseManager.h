#pragma once
#include <JuceHeader.h>

class LicenseManager final
{
public:
    enum class Status
    {
        trial,
        activated,
        expired,
        invalid
    };

    struct Info
    {
        Status status { Status::trial };
        juce::String customerName;
        juce::String customerEmail;
        juce::String licenseKey;
        juce::String machineCode;
        juce::Time firstRun;
        juce::Time activatedAt;
        int trialDays { 14 };
        bool offlineActivation { false };
    };

    LicenseManager();

    void load();
    void save() const;
    void resetTrial();

    bool activateOffline (const juce::String& name,
                          const juce::String& email,
                          const juce::String& key);

    void deactivate();

    Status getStatus() const noexcept { return info.status; }
    Info getInfo() const { return info; }

    bool isActivated() const noexcept { return info.status == Status::activated; }
    bool isTrial() const noexcept { return info.status == Status::trial; }
    bool isExpired() const noexcept { return info.status == Status::expired; }

    int getTrialDaysRemaining() const;
    juce::String getStatusText() const;
    juce::String getMachineCode() const { return info.machineCode; }

    static juce::String statusToString (Status status);
    static juce::String makeOfflineKeyForMachine (const juce::String& name,
                                                   const juce::String& email,
                                                   const juce::String& machineCode);

private:
    Info info;

    juce::File getLicenseFile() const;
    juce::String buildLocalMachineCode() const;
    void updateTrialState();

    static juce::String normalize (juce::String text);
    static juce::String makeHashText (const juce::String& text);
};
