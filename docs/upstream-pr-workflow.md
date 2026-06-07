# Upstream PR Workflow

This is the working checklist for preparing ESPHome and ESPHome docs pull requests from our local forks.

## ESPHome Code PR

1. Sync the fork branch with `esphome/esphome:dev`.
2. Keep the branch name descriptive and avoid tool-specific prefixes.
3. Keep commits scoped and named with ESPHome-style prefixes, for example `[mipi_dsi] ...`.
4. Fill the upstream PR template completely.
5. Use a clear title matching ESPHome conventions, for example `[mipi_dsi] Add DMA2D-backed async flush support`.
6. Link the matching docs PR in the code PR body when the change adds or changes user-facing options.
7. Run or verify the relevant checks before handing off:
   - `pre-commit`
   - component tests for the touched component
   - `clang-tidy` when C++ is touched
   - pylint/custom CI when Python build scripts are touched
8. Treat ESPHome CI bot feedback as the source of truth for labels and required docs/tests.
9. If a helper script is loaded by PlatformIO/SCons, make it import-safe and avoid top-level side effects unless the environment exists.
10. Avoid hardcoded project names, display dimensions, or Atmosfera-specific assumptions in upstreamable code.
11. New hardware-accelerated or behavior-changing options should default to the conservative upstream behavior, usually `false`, and projects that need the feature should enable it explicitly in YAML.

## ESPHome Docs PR

1. Use the docs fork `kyvaith/esphome.io` and branch from the correct upstream docs branch.
2. For docs matching a feature PR in `esphome/esphome`, target `esphome/esphome.io:next`, not `current`.
3. If the docs branch was accidentally based on `current`, rebase only our docs commit onto `next`:

   ```powershell
   git fetch origin next:refs/remotes/origin/next
   git rebase --onto origin/next origin/current <docs-branch>
   git push --force-with-lease fork <docs-branch>
   ```

4. Retarget the docs PR to `next`.
5. If the ESPHome bot converts the PR to draft, mark it ready again after fixing the requested change.
6. Confirm the bot review is dismissed or no longer actionable.
7. Wait for and verify:
   - `Lint`
   - `Build`
   - `Component Index Ordering`
   - Netlify deploy preview
8. Ignore stale cancelled checks from earlier pushes/retargets; evaluate the latest checks for the current head SHA.
9. Keep the docs PR body short and link the matching code PR.

## GitHub Tooling Notes

1. `gh` is not installed in this environment.
2. Prefer the GitHub connector for PR metadata, comments, retargeting, ready-for-review, and status checks where available.
3. Use GitHub REST only when the connector lacks the needed operation.
4. If REST auth is needed, use Git Credential Manager carefully and never print tokens.
5. After push/retarget, always re-check the PR state: base branch, draft state, mergeability, head SHA, and latest checks.
