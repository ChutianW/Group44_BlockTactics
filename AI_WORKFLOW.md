# 🔁 Windsurf Recursive Development Workflow

## 🎯 Purpose
This document defines the mandatory workflow for all development tasks in this repository.

It ensures:
- consistent logic
- minimal bugs
- full-system awareness
- structured improvements
- proper documentation
- compliance with COMP2113 / ENGG1340 project requirements

---

# 🧠 Core Principle

> Never make an isolated change without re-checking the entire repository.

> Never accept an implementation that works technically but violates project requirements.

All changes must go through a recursive loop until:
- the system is stable,
- no bugs remain,
- no requirement violations remain,
- and the project runs correctly.

---

# 🔄 Master Flowchart

```text
    ┌──────────────────────────────┐
    │        NEW PROMPT INPUT      │
    └──────────────┬───────────────┘
                   ↓
    ┌──────────────────────────────┐
    │ 🧠 META-THINKING LEVEL       │
    │ - Identify real goal         │
    │ - Identify task type         │
    │ - Identify affected modules  │
    │ - Predict side effects       │
    └──────────────┬───────────────┘
                   ↓
    ┌──────────────────────────────┐
    │ 🔍 PROBLEM ANALYSIS          │
    │ - Read relevant code         │
    │ - Understand current logic   │
    │ - Identify root cause        │
    └──────────────┬───────────────┘
                   ↓
    ┌──────────────────────────────┐
    │ 🛠️ IMPLEMENT / IMPROVE       │
    │ - Add feature / fix bug      │
    │ - Maintain clean structure   │
    └──────────────┬───────────────┘
                   ↓
    ┌────────────────────────────────────────────┐
    │ 📋 REQUIREMENT CHECK                       │
    │ - Check COMP2113 / ENGG1340 compliance    │
    │ - Verify no requirement is violated       │
    │ - If violated, treat it as a bug          │
    └──────────────┬─────────────────────────────┘
                   ↓
          ┌────────▼────────┐
          │ REQUIREMENT     │
          │ VIOLATION?      │
          └───────┬────────┘
                  │
        ┌─────────┴─────────┐
        │                   │
       YES                  NO
        │                   │
        ↓                   ↓
┌──────────────────────┐   ┌──────────────────────────────┐
│ 🔧 FIX BUG IMMEDIATELY│   │ 🌐 GLOBAL RE-CHECK          │
│ - Solve issue         │   │ - Review entire repo        │
│ - Verify fix          │   │ - Check interactions        │
└──────────┬───────────┘   │ - Validate dependencies     │
           │               └──────────────┬───────────────┘
           ↓                              ↓
   ┌──────────────────────┐   ┌──────────────────────────────┐
   │ 🔁 RECURSIVE RESTART │   │ 🐞 BUG DETECTION             │
   │ Return to beginning  │   │ - Logic / compile errors    │
   │ (Meta-thinking)      │   │ - Runtime issues            │
   └──────────┬───────────┘   │ - Regressions               │
              │               └──────────────┬───────────────┘
              │                              ↓
              │                    ┌────────▼────────┐
              │                    │  BUG FOUND?     │
              │                    └───────┬────────┘
              │                            │
              │              ┌─────────────┴─────────────┐
              │              │                           │
              │             YES                          NO
              │              │                           │
              │              ↓                           ↓
              │    ┌──────────────────────┐   ┌──────────────────────────────┐
              │    │ 🔧 FIX BUG IMMEDIATELY│   │ ✅ COMPLETION CHECK         │
              │    │ - Solve issue         │   │ - Feature complete          │
              │    │ - Verify fix          │   │ - No breaking bugs          │
              │    └──────────┬───────────┘   │ - No requirement violation   │
              │               │               │ - Code compiles              │
              │               ↓               │ - Program runs correctly     │
              │       ┌──────────────────────┐ └──────────────┬──────────────┘
              │       │ 🔁 RECURSIVE RESTART │                ↓
              │       │ Return to beginning  │     ┌──────────────────────────────┐
              │       │ (Meta-thinking)      │     │ 📂 DOCUMENT CHANGES          │
              │       └──────────┬───────────┘     │ - What changed               │
              │                  │                 │ - Why it changed             │
              └──────────────────┴────────────────► - Why it is better           │
                                                    │ - Files affected            │
                                                    └──────────────┬──────────────┘
                                                                   ↓
                                                    ┌──────────────────────────────┐
                                                    │ 🎉 TASK COMPLETE             │
                                                    └──────────────────────────────┘
```
    # 🔁 Recursive Loop Definition
    Fix → Restart → Re-check → Detect → Fix → Restart → ...

    Repeat until:
    - no bugs remain
- all features work correctly
- repository is stable

---

# ⚠️ Non-Negotiable Rules

- ❌ Never make isolated changes  
- ❌ Never ignore side effects  
- ❌ Never continue with known bugs  
- ❌ Never skip full repository re-check  
- ❌ Never mark complete without validation  

---


Repeat until:
- no bugs remain  
- all features work correctly  
- repository is stable  

---

## ⚠️ Non-Negotiable Rules

- ❌ Never make isolated changes  
- ❌ Never ignore side effects  
- ❌ Never continue with known bugs  
- ❌ Never skip full repository re-check  
- ❌ Never mark complete without validation  

---

## 🔍 Step-by-Step Execution

### 1. Meta-Thinking
- Understand the real goal  
- Identify task type (feature / bug / refactor)  
- Identify affected systems  
- Predict side effects  

---

### 2. Problem Analysis
- Read relevant code  
- Understand current logic  
- Identify root cause  
- Avoid blind changes  

---

### 3. Implementation
- Add feature / fix bug  
- Keep code clean and modular  
- Maintain consistency  

---

### 4. Global Re-Check
- Review entire repository  
- Check interactions across modules  
- Validate logic consistency  

---

### 5. Bug Detection
Check for:
- logic errors  
- compile/runtime issues  
- broken dependencies  
- gameplay inconsistencies  
- edge cases  
- regressions  

---

### 6. Fix Bugs Immediately
- Solve bug  
- Verify fix  
- Restart workflow  

---

### 7. Recursive Restart
- Return to meta-thinking  
- Repeat entire process  
- Continue until stable  

---

### 8. Completion Check

A task is complete only if:

- ✔ feature/fix is fully implemented  
- ✔ no known bugs remain  
- ✔ code compiles successfully  
- ✔ program runs correctly  
- ✔ no regressions introduced  
- ✔ repository is consistent  

---

### 9. Documentation Phase

After completion, document all changes.

---

## 📂 Documentation System

### Folder Structure


---

## File Naming
Example: Changes_to_file name_relevant information 


---

## Change Log Template

# Change Record: [Title]

## Date
YYYY-MM-DD

## Type
Bug Fix / Feature / Refactor / Improvement / Documentation

## Files Changed
- file1.cpp
- file2.h

## Problem Before
Describe the issue before this change.

## Change Made
Describe what was modified.

## Why This Change Was Needed
Explain reasoning.

## Why This Version Is Better
Explain improvements.

## Testing / Verification
How you confirmed correctness.

## Notes
Any additional remarks.


🚀 Execution Rule (Summary)
Meta → Analyze → Implement → Re-check → Detect Bugs
       ↓
      Fix → Restart → Repeat → Until Stable → Document


Each record includes:
- What changed
- Files affected
- Problem before
- Why change was needed
- Why it is better
- Verification method

---

## ⚠️ Rules

- Never make isolated changes  
- Never ignore side effects  
- Never build on broken logic  
- Always re-check the entire repository  
- Always document changes  

---

## 🎯 Goal

Ensure every update:
- works correctly
- does not break other parts
- is clearly documented
- improves overall system quality
