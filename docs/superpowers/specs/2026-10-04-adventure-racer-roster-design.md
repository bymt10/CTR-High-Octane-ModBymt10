# Adventure garage full racer roster

**Date:** 2026-10-04  
**Baseline:** Public `vita` branch of `Rinnegatamante/Crash-Team-Racing-High-Octane`, held in a separate local repository.  
**Scope:** Adventure garage selection, racer identity propagation, Oxide availability and stats, and saving that selection.

## Goal

Keep the original eight 3D garage previews and let the player choose any supported racer for Adventure using L1/R1. The selected name and stats update while cycling; the preview model may stay unchanged. Confirming must pass the chosen real racer ID through the existing game flow so the proper character data is used in Adventure hubs, races, boss encounters, scripted sequences where the player racer appears, podiums, and save/load.

The existing project folder remains unchanged. Work and deliverables live in the new independent `CTR-High-Octane-Personal` repository.

## User-visible behavior

- The garage continues to render its existing eight preview models in their original positions.
- L1 and R1 cycle backward and forward through the 16 racer IDs defined by the game's `Characters` enum. The displayed name and stat bars correspond to the pending racer ID; the 3D preview does not need to change.
- Locked racers are skipped using the existing progression flags. Oxide remains unavailable until the original Spyro 2 demo cheat sets the game's Oxide unlock bit.
- Confirm stores the pending racer ID through the existing Adventure selection/save flow. Back, cancel, and the garage's original early-exit behavior remain unchanged.
- Oxide uses the final boss's existing performance data if it can be determined from the game data. If those values cannot be faithfully determined, Speed, Acceleration, and Turn use the maximum displayed values. The bars must show the same values that gameplay uses.

## Design

### Selection state

Keep the garage's visual slot index and the pending racer ID as separate values. The existing eight-slot index continues to drive camera movement and the 3D garage models. The pending racer ID drives the name, stat display, and confirmed player selection. L1/R1 change that ID without changing the eight-slot camera path or modifying `gGarage.garageCharacterIDs`.

Use the game's existing character IDs and metadata rather than custom-racer templates. Build a bounded ordered roster from the 16 IDs in `enum Characters` and use the existing progression unlock requirements for normally unlockable characters. Oxide's availability must be gated by `GAME_UNLOCK_BIT_OXIDE`, which the original Spyro 2 demo cheat sets. Do not make a locked character available merely because its menu entry is visible.

### Gameplay and persistence

On confirmation, assign the selected ID to the existing player character field and `advProgress.characterID`, following the current garage submission path. Let the regular loader and driver systems resolve the model, voice, animation, and engine class from that ID. Check the save/profile entry path and gameplay transitions so an Adventure save reloads the same racer and invalid or locked IDs fall back safely without changing the memory-card layout.

Preserve the story's scripted cast. A player choice controls the player's racer when the game represents that racer; it must not replace boss or cutscene actors whose identity is part of the story.

### Oxide

Leave the Spyro 2 cheat path and its early-exit route intact. Resolve Oxide's final-boss performance from the existing boss/racer metadata. If no faithful values exist, use the engine's maximum stat target for all three values and use those same values for displayed bars and gameplay.

### Compatibility and failure handling

Keep the memory-card schema, default eight garage previews, 60-FPS mode, input bindings, physics, sound, other modes, and unrelated game behavior unchanged. Validate a candidate ID and its unlock state before committing it. When a save contains an invalid or newly locked ID, use the existing safe default racer without writing a malformed character ID back to the save.

## Verification plan

1. Add focused automated coverage for all 16 roster IDs, wraparound, progression lock filtering, the Oxide cheat-bit gate, and saved-ID validation before implementation is considered complete.
2. Build the Windows target and run the repository's CTest smoke test.
3. Build the Vita target and package a VPK when the Vita SDK is available.
4. Exercise every roster entry in a fresh Adventure profile and a loaded profile, checking name, stats, actual in-game model/voice/animation, hub entry, normal race, boss race, player-in-scene transitions, podium, and save/reload. Check Oxide before and after its cheat. Confirm all existing garage previews remain unchanged.
5. Check existing 60-FPS and controller behavior in the same sessions. Report any scenario that cannot be run on the available Windows/Vita hardware or toolchain; do not describe it as verified.

## Acceptance criteria

- The original directory is unchanged and the source project is a separate, editable Git checkout.
- All 16 racer IDs are reachable from the eight-slot Adventure garage using L1/R1 when their existing unlock requirement is satisfied; Oxide requires the original cheat.
- The selected ID is the one used by gameplay and survives Adventure save/load.
- Oxide's exit route is preserved and his bars and driving values match the resolved boss values or the explicit maximum fallback.
- Windows and Vita deliverables build successfully, with the exact hardware validation coverage reported.
- No unrelated refactor or behavior change is included.
