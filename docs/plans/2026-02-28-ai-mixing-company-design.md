# AI Mixing Assistant Company Design

## 1. Mission / Vision / Principles

### Mission
- We support home recordists' artistic senses and build a more comfortable, premium music production environment while reducing financial and physical burden.

### Vision
- Become a creative companion that preserves artistic intent, not a replacement tool that forces technical decisions.

### Product Promise
- Translate sensory language (e.g., "muddy", "more body", "fluffy but less clumped") into actionable audio processing.
- Prioritize quality and trust: if confidence is low, do not auto-apply aggressive changes.
- Operate under clear performance goals:
  - Target processing time: within 2 minutes
  - Hard cap: within 5 minutes

### Red Lines
- No "one-click miracle" marketing claims.
- No guarantee of direct artist-style cloning.
- No opaque black-box changes without explanation.

## 2. Target Market and Product Focus

### Primary Customer
- Home recording and indie artists.

### Initial Product Form
- macOS + Logic Pro + AU plugin.

### Core Value Priority
- Main axis: conversational mixing copilot (natural language -> explainable suggestions).
- Must include over time:
  - (1) Muddy/dull sound correction
  - (2) Conversational copilot experience (primary)
  - (3) Reference-driven similarity guidance (with safety constraints)

## 3. Operating Model (1-person centered, occasional 2nd member)

### Functional Roles
- Product: user interviews, prioritization, success metrics.
- Audio R&D: sensory vocabulary to acoustic target mapping.
- Engineering: AU plugin implementation, inference pipeline, performance.
- GTM: landing page, demos, onboarding, pricing experiments.

### Expansion Split (when 2nd member joins)
- Founder: Product + Audio R&D.
- Partner: Engineering + release/quality automation.

### Weekly Cadence
- Monday: synthesize user feedback, define 1-2 experiments.
- Tuesday-Thursday: build, run listening tests, regression checks.
- Friday: release demo build, capture metrics, reprioritize backlog.

## 4. Decision Framework

All product decisions are evaluated in this order:
1. Visual usability: Does the plugin present auditory information in an intuitively readable way?
2. Quality improvement power: Can it improve results regardless of user ear-training level?
3. Mix-wide balance awareness: Does it optimize across the whole track context, not isolated channels only?
4. Time and reliability: Can it hit target (2 min) and hard cap (5 min) consistently?
5. Explainability: Can users understand why the system suggested or applied a change?

## 5. Product Roadmap (12 Months)

### 0-3 Months: AU MVP for Logic
- Deliver "muddy sound correction" and minimal conversational input set.
- Support proposal-first UX:
  - Intent interpretation
  - What/why visualization
  - A/B preview before apply

### 4-6 Months: Copilot Expansion
- Expand sensory vocabulary map (body, clarity, de-clumping, density control).
- Add mix-wide balance indicators (e.g., vocal presence, low-end crowding risk).

### 7-9 Months: Controlled Reference Feature
- Introduce tone-profile similarity guidance.
- Avoid direct artist-copy positioning and enforce safety language in product messaging.

### 10-12 Months: Monetization and Reliability
- Run pricing tests (free tier + pro subscription + studio tier).
- Strengthen crash/performance/quality regression pipelines.

## 6. Preparation Checklist

### Product and Research
- Interview at least 20 target users about "muddy" and "clarity" vocabulary patterns.
- Build a rubric for before/after listening sessions.

### Data and Evaluation
- Build internal dataset: stems + mixes across genres with controlled test cases.
- Define quality gates: subjective listening + objective audio metrics.

### Technical Foundation
- AU-first architecture for Logic on macOS.
- Local-first processing with optional cloud-assisted components.

### Legal and Messaging
- Keep reference feature phrasing as "similar tone guidance."
- Explicitly avoid "exact clone" promises.

## 7. 30-Day Action Plan

1. Week 1: finalize interview script and quality rubric.
2. Week 2: build two prototype chains for muddy-sound correction.
3. Week 3: implement AU MVP shell with chat input + A/B + rationale panel.
4. Week 4: run 10-user pilot and measure satisfaction, reuse intent, and speed-to-result.

## 8. Success Metrics (Early)

- Time-to-first-usable result <= 2 minutes (hard cap <= 5 minutes).
- Perceived quality improvement score (user listening test) above baseline.
- Reuse rate: users re-running copilot on new sessions.
- Edit acceptance rate: ratio of suggested changes that users keep.
