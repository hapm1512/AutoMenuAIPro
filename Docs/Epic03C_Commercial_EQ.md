# Epic 3C - Commercial EQ

Status: Source integrated.

## Implemented
- Commercial EQ module replacement.
- HPF stage.
- Low shelf.
- Low-mid bell.
- Mid bell.
- High-mid bell.
- High shelf.
- Air shelf.
- Dynamic high-mid control.
- M/S blend processing.
- Auto gain compensation.
- Output trim.
- Parameter smoothing.
- Backward-compatible legacy EQ params.

## Main parameters
- eqHpfFreq
- eqLowGain / eqLowFreq
- eqLowMidGain / eqLowMidFreq / eqLowMidQ
- eqMidGain / eqMidFreq / eqMidQ
- eqHighMidGain / eqHighMidFreq / eqHighMidQ
- eqHighGain / eqHighFreq
- eqAirGain / eqAirFreq
- eqDynAmount / eqDynThreshold
- eqMS
- eqAutoGain
- eqOutput

## DSP chain
Input -> HPF -> Low Shelf -> Low Mid -> Mid -> High Mid Dynamic -> High Shelf -> Air -> Auto Gain -> Output
