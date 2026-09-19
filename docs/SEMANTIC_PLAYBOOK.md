# Semantic reconstruction playbook

This playbook adapts the successful TH08 reconstruction method to TH095. It
transfers questions, evidence discipline, and closure tests—not TH08's source
names, class layouts, opcode meanings, or completion claims.

## 1. Ask a semantic question

Do not begin with “what can be renamed?” Begin with one behavior question:

- What owns this storage throughout its lifetime?
- Which transition changes this state, and what independently consumes it?
- Is this number an enum, an identifier, a bit mask, an index, or serialized
  wire data?
- Is a local `*View` revealing a real subobject or merely projecting one call?
- Which source body enters the exact object, the normal product, and any future
  portable product?

A useful batch has a bounded owner, protocol, or transition and a falsifiable
exit condition. “Improve semantics in this file” is not bounded.

## 2. Build an evidence packet before editing

For the selected address or storage family, collect:

1. the verified target identity and function ledger row;
2. target disassembly/decompile for every relevant producer and consumer;
3. xrefs, callers, callees, constants, strings, and relocation destinations;
4. access width, signedness, offset, initialization, mutation, and destruction;
5. the source declarations and every duplicate view or owner;
6. exact-unit ownership and the normal whole-build owner/profile;
7. adjacent-game evidence, clearly labeled as corroboration; and
8. the smallest exact and normal baseline commands.

Use a producer and an independent consumer whenever possible. A lone write can
usually establish storage shape, not meaning. For state machines, close the
transition set rather than naming one attractive value.

## 3. Separate four kinds of ownership

TH095 often makes these different:

| Ownership axis | Question |
| --- | --- |
| Storage | Which aggregate allocation physically contains the bytes? |
| Semantic | Which subsystem defines their lifetime and invariants? |
| Emission | Which TU/source shape makes VC7 emit the target body? |
| Build product | Which declaration/body is compiled into exact, normal, and future portable products? |

A class-qualified symbol does not prove TU ownership. A local view does not
prove storage ownership. A matching probe does not become the production
owner. Record all four axes before consolidating declarations.

## 4. Recover representation before interpretation

First establish the physical contract:

- scalar width and signedness;
- arrays, counts, strides, and sentinels;
- embedded versus pointed-to ownership;
- constructor/destructor and allocation size;
- file/instruction wire shape;
- vtable, callback, and calling-convention position; and
- `sizeof`/`offsetof` facts used by target code.

Then interpret roles using behavior. This ordering prevents a plausible name
from forcing the wrong C++ representation. Byte-oriented wire data should stay
byte-oriented; a runtime aggregate with stable members should not remain a raw
byte blob merely because the exact profile once needed a view.

## 5. Use a conservative naming ladder

Prefer the strongest name supported by current evidence:

1. **Structural:** `unknown010`, `reserved`, `storage`, byte range.
2. **Neutral role:** `threadHandle`, `inputCursor`, `stageInstruction`.
3. **Protocol role:** `playbackMode`, `cameraMotionMode`, `captureActive`.
4. **Domain role:** only after producers, consumers, and transitions agree.
5. **Historical/original identifier:** only with direct provenance; otherwise
   do not imply recovery of ZUN's spelling.

When two roles compete, keep the neutral name and record the unresolved
alternatives. Renaming is evidence publication, not aesthetic cleanup.

## 6. Treat protocols and namespaces as first-class objects

Numbers can belong to different namespaces even when their values overlap:

- instruction opcodes;
- operand selectors;
- physical object flags;
- state-machine modes;
- resource file slots;
- script or sprite identifiers;
- VM pool indices;
- serialized version/format fields; and
- callback or scheduler priorities.

Do not merge namespaces because two fields both contain `8`. Close a protocol
by identifying its complete domain, producers, consumers, invalid/default
behavior, and representation. Add `scripts/check-semantic-protocols.py` rules
only for surfaces that are already closed; the guard preserves evidence-backed
vocabulary and must not fabricate names for open protocols.

Protocol debt is often invisible to offset-based scanning. After the heuristic
router, explicitly inspect dispatch tables, comparisons, masks, state writes,
and serialization boundaries.

## 7. Use views as scaffolding, then converge ownership

A small view is appropriate while proving one access family. It becomes debt
when production contains multiple full or overlapping definitions of the same
runtime object, when casts are the ordinary access path, or when different TUs
silently disagree about a live layout.

For consolidation:

1. identify the real allocation and lifetime owner;
2. enumerate every declaration and cast projection;
3. compare layouts and compile profiles;
4. move target-proven fields into one canonical declaration;
5. keep only narrow views for genuine wire/ABI boundaries;
6. isolate any exact-only source-shape compatibility in the smallest scope;
7. replay all affected exact objects and rebuild normal production.

Do not declare success merely because all views share the same size. The
canonical owner must be the type normal code actually uses.

## 8. Handle exact/normal divergence explicitly

TH095 currently has many `TH095_MATCH_EXACT`, `DIFFBUILD`, and `*Exact.inl`
boundaries. For each selected batch classify the relationship:

- **Same body:** exact replay also covers the normal semantic body, subject to
  profile flags and declarations.
- **Token-only compatibility:** bodies are logically shared but a name/macro
  differs; exact replay covers emission, while target evidence and normal
  compile support the normal token.
- **Different body:** exact replay says nothing about the normal function body.
  Validate normal semantics independently.
- **Exact-only duplicate owner:** treat as compiler/emission scaffolding, never
  as the portable implementation.

Prefer deletion of unnecessary divergence. Retain a split only when a minimal
VC7 oracle and target comparison show that natural shared C++ cannot preserve
the required emission. Document that boundary beside the source.

## 9. Use independent oracles

The exact oracle answers “did pinned VC7.1 reproduce the configured target
bytes and relocations?” It does not answer whether a normal-only name or body
is correct.

The normal/portable oracle answers “does the maintained semantic source compile
and behave through a different build path?” TH095 currently has normal VC7.1
whole-product compile/link closure, which catches ownership and declaration
breakage but is not yet an independent modern-compiler/runtime oracle.

Use `th095-oracle` style experiments for ambiguous source shape:

- one hypothesis per experiment;
- minimal synthetic source;
- pinned compiler and flags recorded;
- object/disassembly result saved below `.analysis/`;
- conclusion limited to what the experiment distinguishes.

Escalate to a runtime scenario when the claim concerns state transitions,
lifetime, callbacks, rendering, persistence, or error handling.

## 10. Close the right thing

Before accepting a batch, check six closures:

1. **Representation:** widths, layout, storage, wire/runtime distinction.
2. **Consumers:** all relevant readers/writers, not one convenient function.
3. **Protocol:** domain, transitions, invalid/default path, namespace.
4. **Ownership:** storage, semantic, emission, and build-product owners.
5. **Oracle:** exact and normal/portable evidence appropriate to the change.
6. **Memory:** durable KB/history/source comments say what is proven and what
   remains Unknown.

The batch can close while the subsystem remains open. Say so plainly.

## 11. Common failure modes

- Chasing the next `unknown` name without closing an owner or protocol.
- Treating `696/696 exact` as validation of normal-only bodies.
- Copying TH08 names or layouts into TH095 because the games look related.
- Promoting a decompiler type or auto-name into source without target dataflow.
- Adding a new view instead of reconciling the real aggregate.
- Turning serialized bytes into host-native structs without proving packing and
  ownership.
- Using debt counts as semantic percentages.
- Appending session diaries to the current handoff.
- Broadly changing shared headers before capturing affected exact units.
- Keeping compiler workarounds without a bounded oracle and source comment.

## 12. Documentation topology

| File | Durable purpose |
| --- | --- |
| `SEMANTIC_RECONSTRUCTION.md` | Short current policy and acceptance rules |
| `SEMANTIC_PLAYBOOK.md` | Reusable method distilled from TH08 experience |
| `SEMANTIC_INDEX.md` | Current concept-to-owner/evidence navigation |
| `SEMANTIC_HISTORY.md` | Chronological accepted batch records |
| `SOURCE_MAP.md` | Exact/normal/emission/build ownership map |
| `KNOWLEDGE_BASE.md` | Stable target facts with evidence provenance |
| `RE_HANDOFF.md` | Replaceable current status, blockers, and next lane |
| `RE_HANDOFF_HISTORY.md` | Archived prior operational handoffs |

Historical prose can be wrong or superseded. Current policy, ledgers, target
evidence, and the short handoff take precedence.

## 13. First TH095 pilot: Background

The initial canonical-owner pilot is `Background` because current production
has a method-owning `Background` byte blob, a separate full 0x201C state view,
and a legacy header layout used by other compile paths. The pilot must:

- re-attest target allocation/lifetime and the 0x201C layout;
- map every declaration, cast, and TU consumer;
- make the normal build use one real canonical 0x201C owner;
- preserve the complete stage-script opcode namespace;
- keep only a narrow, evidenced VC7 emission boundary where source unification
  changes accepted bytes; and
- replay every affected exact unit plus the normal whole product.

The pilot does not authorize copying TH08's `Background` layout or extending
the repository's narrow assembly exception.
