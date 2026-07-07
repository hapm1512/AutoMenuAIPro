# Milestone B - Live Suggestion + Apply

Scope:
- Adds SuggestionEngine.
- Adds TonePresetMapping.
- Adds SuggestedPreset state.
- Updates AutoMenuCore with updateSuggestion/applyCurrentSuggestion.
- Updates ToneDetectorComponent to show Suggested Preset and APPLY.
- Dashboard now updates suggestion and applies it through Core.

Build:
cmake -S . -B Build
cmake --build Build --config Release

Commit:
git add .
git commit -m "Milestone B - Add live suggestion and apply workflow"
git push
git tag milestone-b-live-suggestion-v1
git push origin milestone-b-live-suggestion-v1
