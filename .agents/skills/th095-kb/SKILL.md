---
name: th095-kb
description: Maintain the TH095 verified knowledge base, evidence provenance, current handoff, generated progress, and conservative tracking documentation. Use when recording reverse-engineering findings, resolving stale documentation, preparing a session handoff, or changing reconstruction status claims.
---

# TH095 knowledge maintenance

Treat ledgers as live state and prose as explanation.

## Route information

- Put target-wide durable facts in `docs/KNOWLEDGE_BASE.md` with an evidence
  class and reproducible source.
- Put the current state, blocker, and next bounded lane in
  `docs/RE_HANDOFF.md`; replace stale handoff text instead of appending a diary.
- Put operating method in `docs/RE_WORKFLOW.md`, oracle rules in
  `docs/ORACLES.md`, and compiler lessons in `docs/BUILD_MATCHING.md`.
- Put address/name/origin/source/exact state in `config/`, never only in prose.
- Keep experiments and rejected hypotheses below `.analysis/` unless a concise
  durable lesson prevents future repetition.

## Update safely

1. Read the relevant ledgers and current target evidence.
2. Label each claim observed, compiler-observed, corroborated, inferred, or
   unknown.
3. Include exact address, target hash context, command/report, and date when
   they materially affect reproducibility.
4. Do not copy live counts into prose manually. Run `scripts/progress.py` and
   `scripts/report-reconstruction-status.py --summary`.
5. Run `scripts/validate-tracking.py` and `scripts/ci.py` before commit.

Never promote source-present or semantic acceptance to exactness without the
canonical strict comparison required by `$th095-matching`.
