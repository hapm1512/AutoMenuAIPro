# Vocal Suite Ultra Pro — User Manual

Author: Hai Pham Audio
Version: 4.5.0 RC

## 1. Overview
Vocal Suite Ultra Pro is a vocal processing plugin designed for live vocal, studio recording, karaoke, podcast, broadcast, and creative vocal color.

The plugin contains a full vocal chain:

1. Analog Front-End
2. Professional Gate
3. Commercial EQ
4. Commercial De-Esser
5. 1176 Compressor
6. LA-2A Compressor
7. Analog Color
8. Output Stage
9. Professional Metering
10. Preset System

## 2. Recommended Signal Flow
Place the plugin directly on the vocal track.

Recommended input level:

- Normal vocal peak: -18 dBFS to -10 dBFS
- Loud vocal peak: below -6 dBFS
- Avoid clipping before the plugin

## 3. Analog Front-End
This stage prepares the vocal before heavy processing.

Main use:

- Remove DC offset
- Control input trim
- Add analog headroom
- Add transformer or tube style tone
- Protect against overload

Suggested use:

- Use less drive for clean vocal.
- Use more drive for thicker live vocal.
- Avoid too much drive on already distorted recordings.

## 4. Professional Gate
The gate reduces background noise between vocal phrases.

Main use:

- Reduce room noise
- Reduce mic noise
- Clean pauses
- Preserve vocal tail

Suggested use:

- Start with a light gate.
- Increase threshold only until noise is reduced.
- Avoid cutting the ends of words.

## 5. Commercial EQ
The EQ shapes the vocal tone.

Common adjustments:

- High-pass: remove rumble.
- Low shelf: control body.
- Mid bands: reduce boxiness or nasal tone.
- High shelf: add clarity.
- Air band: add openness.

Suggested use:

- Cut before boosting.
- Use small moves first.
- Keep air band moderate for live vocal.

## 6. Commercial De-Esser
The de-esser controls harsh S and SH sounds.

Suggested use:

- Start around 5 kHz to 8 kHz.
- Use split-band for natural vocal.
- Use wide-band only for strong sibilance.

## 7. 1176 Compressor
The 1176 stage adds fast control and vocal presence.

Suggested use:

- Use faster attack for aggressive control.
- Use slower attack for punch.
- Keep gain reduction moderate.

## 8. LA-2A Compressor
The LA-2A stage adds smooth leveling.

Suggested use:

- Use after 1176.
- Aim for gentle leveling.
- Good for warm, stable vocal.

## 9. Analog Color
This stage adds tone and harmonic character.

Main use:

- Tape saturation
- Tube color
- Transformer weight
- Presence excitation
- Air enhancement
- Stereo width

Suggested use:

- Use subtle settings for commercial vocal.
- Increase color for creative effects.
- Check output level after saturation.

## 10. Output Stage
The output stage controls final loudness and safety.

Main use:

- Output trim
- Soft clip protection
- True peak limiting
- Ceiling control
- Dither
- Meter tap

Suggested use:

- Keep ceiling below 0 dBFS.
- Use true peak protection for final safety.
- Use dither only when rendering to lower bit depth.

## 11. Metering
The metering section helps monitor the vocal signal.

Meters may include:

- Peak
- RMS
- LUFS Momentary
- LUFS Short-Term
- LUFS Integrated
- True Peak
- Stereo Correlation
- Gain Reduction

Suggested use:

- Watch input and output peaks.
- Keep output from clipping.
- Compare processed and bypassed level.

## 12. Presets
Factory presets are grouped by category.

Categories:

- Studio
- Live
- Podcast
- Broadcast
- Creative

Use presets as starting points. Adjust input trim, gate, EQ, and output to match the singer and microphone.

## 13. Bypass
Use bypass to compare processed and unprocessed vocal.

Best practice:

- Match loudness before judging tone.
- Do not judge a preset only by louder output.

## 14. Automation
Most controls can be automated in the DAW.

Recommended automation:

- Output gain
- Mix amount
- Preset changes only between song sections
- Avoid extreme fast automation on heavy DSP parameters

## 15. CPU Tips
To reduce CPU load:

- Use larger buffer sizes for mixing.
- Use lower oversampling where available.
- Disable unused modules.
- Freeze or render tracks when needed.

## 16. Safety Notes
Avoid clipping before and after the plugin.
Always test live settings at performance volume before showtime.
