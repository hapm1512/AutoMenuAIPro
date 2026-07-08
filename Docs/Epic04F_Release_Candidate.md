# Epic 4F - Release Candidate

## Goal
Prepare Vocal Suite Ultra Pro for release candidate testing after Epic 1-4E.
This epic does not change the core DSP chain. It adds release packaging, versioning,
benchmark checklist, changelog, and final release validation support.

## Scope
- Release candidate version metadata
- Changelog template
- Release checklist
- Package script foundation
- Smoke test checklist
- Installer preparation notes
- VST3 artifact collection guide
- Final QA sign-off document

## Release Candidate Definition
A build can be marked RC only when:

- CMake Release build succeeds
- VST3 loads in target DAWs
- Standalone target opens
- Presets load and save
- Bypass is click-free
- Automation does not crash
- CPU is stable at common buffer sizes
- No NaN / Inf output is detected
- Output limiter prevents hard clipping
- GUI repaint is stable under resize

## Target tag

```text
v4.5-epic4f-release-candidate
```

## Recommended commit

```text
Epic 4F Release Candidate
```
