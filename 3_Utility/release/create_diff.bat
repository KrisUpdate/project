REM GIT_COMMIT and GIT_PREVIOUS_COMMIT are variables of Jenkins
git diff %GIT_COMMIT% %GIT_PREVIOUS_COMMIT% > changes.diff