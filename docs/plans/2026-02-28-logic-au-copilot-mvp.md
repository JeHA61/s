# Logic AU Conversational Copilot MVP Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Build a macOS Logic Pro AU plugin MVP that converts Korean/English sensory text into explainable "muddy sound" correction suggestions with A/B audition in <=2 minutes target (<=5 minutes hard cap).

**Architecture:** Use JUCE for AU plugin shell and UI, with a local C++ analysis/suggestion engine. Keep inference deterministic in v1 using rule-based intent parsing and DSP presets, then add optional learned ranking later. Persist each suggestion with rationale and rollback-safe snapshots.

**Tech Stack:** C++17, JUCE 8, CMake, Catch2, macOS AU validation tools (`auval`), optional Python scripts for offline metric checks.

---

### Task 1: Bootstrap AU Plugin Project (Logic-First)

**Files:**
- Create: `CMakeLists.txt`
- Create: `src/PluginProcessor.h`
- Create: `src/PluginProcessor.cpp`
- Create: `src/PluginEditor.h`
- Create: `src/PluginEditor.cpp`
- Create: `README.md`

**Step 1: Write the failing build expectation**

```bash
cmake -S . -B build
```

Expected: FAIL because project files do not exist yet.

**Step 2: Write minimal project scaffolding**

```cmake
cmake_minimum_required(VERSION 3.24)
project(MixCopilotAU VERSION 0.1.0)
```

**Step 3: Configure JUCE target and AU format**

```cmake
juce_add_plugin(MixCopilotAU
    COMPANY_NAME "MixCopilot"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    FORMATS AU
    PRODUCT_NAME "MixCopilot")
```

**Step 4: Run configure/build and verify pass**

```bash
cmake -S . -B build && cmake --build build -j
```

Expected: PASS with AU target generated.

**Step 5: Commit**

```bash
git add CMakeLists.txt src/ README.md
git commit -m "chore: bootstrap JUCE AU plugin project for Logic"
```

### Task 2: Add Audio Feature Extraction Module with Tests

**Files:**
- Create: `src/audio/FeatureExtractor.h`
- Create: `src/audio/FeatureExtractor.cpp`
- Create: `tests/audio/test_feature_extractor.cpp`
- Modify: `CMakeLists.txt`

**Step 1: Write the failing test**

```cpp
TEST_CASE("FeatureExtractor returns spectral centroid and low-mid energy ratio")
{
    FeatureExtractor ex;
    auto features = ex.analyze(makeSineBuffer(220.0f));
    REQUIRE(features.spectralCentroidHz > 0.0f);
    REQUIRE(features.lowMidRatio >= 0.0f);
}
```

**Step 2: Run test to verify failure**

```bash
ctest --test-dir build --output-on-failure
```

Expected: FAIL (missing extractor symbols).

**Step 3: Write minimal implementation**

```cpp
struct MixFeatures { float spectralCentroidHz {}; float lowMidRatio {}; };
```

Add deterministic analysis for centroid and 150-500 Hz ratio.

**Step 4: Run tests and verify pass**

```bash
cmake --build build -j && ctest --test-dir build --output-on-failure
```

Expected: PASS.

**Step 5: Commit**

```bash
git add src/audio tests/audio CMakeLists.txt
git commit -m "feat: add feature extraction module with basic spectral metrics"
```

### Task 3: Implement Muddyness Detector (Rule-Based V1)

**Files:**
- Create: `src/analysis/MuddyDetector.h`
- Create: `src/analysis/MuddyDetector.cpp`
- Create: `tests/analysis/test_muddy_detector.cpp`

**Step 1: Write failing detector test**

```cpp
TEST_CASE("MuddyDetector flags crowded low-mid content")
{
    MuddyDetector detector;
    auto score = detector.score(makeLowMidHeavyBuffer());
    REQUIRE(score > 0.65f);
}
```

**Step 2: Run to confirm failure**

```bash
ctest --test-dir build -R muddy --output-on-failure
```

Expected: FAIL (detector missing).

**Step 3: Implement minimal score logic**

```cpp
float score = clamp(lowMidRatio * 0.7f + maskingIndex * 0.3f, 0.0f, 1.0f);
```

Define thresholds:
- `>0.65`: high muddy risk
- `0.45-0.65`: medium
- `<0.45`: low

**Step 4: Re-run tests**

```bash
cmake --build build -j && ctest --test-dir build -R muddy --output-on-failure
```

Expected: PASS.

**Step 5: Commit**

```bash
git add src/analysis tests/analysis
git commit -m "feat: add muddyness detector with deterministic risk scoring"
```

### Task 4: Build Conversational Intent Parser (Korean/English Core Phrases)

**Files:**
- Create: `src/nlp/IntentParser.h`
- Create: `src/nlp/IntentParser.cpp`
- Create: `tests/nlp/test_intent_parser.cpp`

**Step 1: Write failing parser tests**

```cpp
REQUIRE(parse("소리가 너무 먹먹해").intent == Intent::ReduceMuddiness);
REQUIRE(parse("make it more clear and punchy").intent == Intent::IncreaseClarity);
```

**Step 2: Run and verify failure**

```bash
ctest --test-dir build -R intent --output-on-failure
```

Expected: FAIL.

**Step 3: Implement minimal phrase map**

```cpp
if (containsAny(text, {"먹먹", "muddy", "boxy"})) return Intent::ReduceMuddiness;
```

Add confidence score and fallback `Intent::Unknown`.

**Step 4: Re-run tests**

```bash
cmake --build build -j && ctest --test-dir build -R intent --output-on-failure
```

Expected: PASS.

**Step 5: Commit**

```bash
git add src/nlp tests/nlp
git commit -m "feat: add bilingual intent parser for sensory phrases"
```

### Task 5: Add Suggestion Engine (Explainable Proposal, Not Forced Auto-Apply)

**Files:**
- Create: `src/engine/SuggestionEngine.h`
- Create: `src/engine/SuggestionEngine.cpp`
- Create: `tests/engine/test_suggestion_engine.cpp`

**Step 1: Write failing test for rationale output**

```cpp
REQUIRE(suggestion.reason.find("low-mid") != std::string::npos);
REQUIRE(suggestion.actions.size() > 0);
```

**Step 2: Run and confirm failure**

```bash
ctest --test-dir build -R suggestion --output-on-failure
```

Expected: FAIL.

**Step 3: Implement minimal suggestion rule set**

```cpp
// Example actions
// - Bell cut -2.5 dB at 280 Hz (Q 1.2)
// - Dynamic EQ sidechain for vocal band protection
// - Gentle transient enhancement +5%
```

Always produce:
- `reason` (why)
- `impact` (expected result)
- `confidence` (0-1)

**Step 4: Re-run tests**

```bash
cmake --build build -j && ctest --test-dir build -R suggestion --output-on-failure
```

Expected: PASS.

**Step 5: Commit**

```bash
git add src/engine tests/engine
git commit -m "feat: add explainable suggestion engine for muddy correction intent"
```

### Task 6: Build UI Flow (Prompt -> Explain -> A/B)

**Files:**
- Modify: `src/PluginEditor.h`
- Modify: `src/PluginEditor.cpp`
- Create: `src/ui/RationalePanel.h`
- Create: `src/ui/RationalePanel.cpp`
- Create: `tests/ui/test_rationale_format.cpp`

**Step 1: Write failing UI-format test**

```cpp
REQUIRE(formatReason("low-mid crowded").find("왜") != std::string::npos);
```

**Step 2: Run and verify failure**

```bash
ctest --test-dir build -R rationale --output-on-failure
```

Expected: FAIL.

**Step 3: Implement minimal editor widgets**

```cpp
// Components:
// - Text input (user prompt)
// - "Analyze" button
// - Suggestion list with rationale
// - A/B toggle
// - Apply / Rollback buttons
```

**Step 4: Re-run tests/build**

```bash
cmake --build build -j && ctest --test-dir build -R rationale --output-on-failure
```

Expected: PASS.

**Step 5: Commit**

```bash
git add src/PluginEditor.* src/ui tests/ui
git commit -m "feat: add conversational UI with rationale panel and A/B controls"
```

### Task 7: Add Safe Apply/Rollback and Time Budget Guard

**Files:**
- Create: `src/engine/SessionState.h`
- Create: `src/engine/SessionState.cpp`
- Create: `tests/engine/test_session_state.cpp`
- Modify: `src/PluginProcessor.cpp`

**Step 1: Write failing rollback test**

```cpp
REQUIRE(state.canRollback());
state.rollback();
REQUIRE(state.currentChain() == state.originalChain());
```

**Step 2: Run and confirm failure**

```bash
ctest --test-dir build -R session --output-on-failure
```

Expected: FAIL.

**Step 3: Implement state snapshots + time guards**

```cpp
constexpr auto kTargetMs = 120000; // 2 min
constexpr auto kHardCapMs = 300000; // 5 min
```

Behavior:
- If target exceeded: show "fast fallback suggestion mode".
- If hard cap exceeded: stop heavy analysis and return safe minimal proposal.

**Step 4: Re-run tests**

```bash
cmake --build build -j && ctest --test-dir build -R session --output-on-failure
```

Expected: PASS.

**Step 5: Commit**

```bash
git add src/engine/SessionState.* src/PluginProcessor.cpp tests/engine
git commit -m "feat: add safe apply/rollback and 2-min target with 5-min hard cap guardrails"
```

### Task 8: Verification, AU Validation, and Pilot Build

**Files:**
- Create: `scripts/verify/audio_regression.py`
- Create: `docs/testing/logic-au-validation.md`
- Modify: `README.md`

**Step 1: Write failing verification check**

```bash
python3 scripts/verify/audio_regression.py --input fixtures --baseline baselines
```

Expected: FAIL (script/file missing).

**Step 2: Implement minimal verification script**

```python
# Check before/after low-mid energy reduction and loudness delta bounds.
```

**Step 3: Run AU validation**

```bash
auval -a | grep -i MixCopilot
```

Expected: plugin is listed and AU validation passes.

**Step 4: Run full verification**

```bash
cmake --build build -j
ctest --test-dir build --output-on-failure
python3 scripts/verify/audio_regression.py --input fixtures --baseline baselines
```

Expected: all checks PASS.

**Step 5: Commit**

```bash
git add scripts/verify docs/testing README.md
git commit -m "chore: add verification pipeline and Logic AU validation checklist"
```

## Definition of Done

- Logic recognizes the AU plugin.
- User can type a sensory phrase and receive explainable suggestions.
- User can A/B compare and rollback safely.
- Muddy correction workflow achieves <=2 minute typical turnaround and never exceeds 5-minute hard cap.
- Test suite and verification script pass on macOS target environment.
