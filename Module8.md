# **Day 8 - Game Jam**

## **The brief**

You have five days, days 08 to 12, to build a game, solo, in C++, that shows off everything from Week 1.

## **Theme: Malaysia Day**

Any genre. The theme is Malaysia Day.

Read it as broadly as you like: 16 September 1963 and the forming of Malaysia, or unity, or coming together, or many separate parts becoming one thing. Literal or abstract, historical or entirely invented. A puzzle game about joining pieces is as valid as a strategy game set in 1963.

Nobody is grading historical accuracy. The theme is a creative constraint to push your idea somewhere specific.

### **Constraints**

- Solo. One person, one game. No teams.
- Language: C++17, built with `Wall -Wextra -Werror` (same bar as Week 1).
- No game engines. Unity, Unreal and Godot are out, along with anything else where an editor owns the project. You may link against a library you drive from your own `main`: raylib, SFML, SDL2, raw OpenGL, or terminal-only (`ncurses` / ANSI escape codes). State what you used and why in your README. The one rule that hasn't changed: the game logic is yours. Don't submit a tutorial project with the gameplay already written for you.
- Tools are yours. If your game needs supporting tooling, a level editor, a map or asset converter, a tilemap format, a debug overlay, you write it (if you've got the time). No Tiled, no off-the-shelf editor feeding your game. Rendering, input and audio still come from the library you picked. The pipeline around them doesn't.
- Build: a `Makefile` with `all`, `clean`, `fclean`, `re` that produces a runnable executable. It must build from a clean checkout on your own machine, so pin any library versions.
- Scope: one complete core loop or proof-of-concept feature beats a broad, unfinished pile of features. Aim for a playable core loop by the end of day 09, then deepen.
- Done looks like: it builds warning-clean, runs without crashing or leaking, has a README a stranger can follow, and the code visibly puts Week 1 to work (see below).

## **What we're looking for**

You're building a game, but what gets judged is the engineering and the creativity behind it. Across your code we expect to see:

- Const-correct, cleanly structured code (Day 1)
- No leaks; ownership expressed through RAII / smart pointers (Day 2)
- A sensible class design, with polymorphism where it earns its place (Day 3)
- Appropriate STL containers (Day 4)
- Patterns used with judgement, and with restraint (Day 5)
- Real algorithms, for example pathfinding, if it fits (Day 6)
- Tests guarding the parts that matter (Day 7)

Reach for these while you're building the game. Don't build the game around the list. And make sure the game is actually fun.

## **Rhythm of the week**

| Day | Focus | Checkpoint |
| --- | --- | --- |
| 08 | Concept, scope, project skeleton | - |
| 09 | Core loop playable | Peer check-in 1: concept & plan |
| 10 | Build features | - |
| 11 | Build / polish | Peer check-in 2: progress & problems |
| 12 | Demo day: finish, document, submit | **Submission closes 5:00 PM** |

## **Two evaluations (check-ins, conversations, not graded)**

These are peer check-ins run with another learner. They are not gates and there is nothing to pass. They exist so you spend half an hour thinking out loud with someone who's building something too:

- bounce your idea off another person and see what they poke at
- talk through the library you picked and why it fits what you're making
- say which techniques you're trying to show off in this project
- put your current struggle on the table so two people are looking at it instead of one

Say what's actually stuck. That's what makes the half hour worth spending.

- Day 09, concept & plan: walk a peer through your idea, your theme angle, your library choice, and your plan.
- Day 11, progress & problems: demo your progress, share your struggles, and work out a next thing together.

## **Submission: day 12, 5:00 PM**

Day 12 is demo day. Submission closes at **5:00 PM sharp** and no pushing is allowed after that. The commit at the top of your public repo at 5:00 PM is the one that gets read. Plan for it: stop adding features on day 12 morning and spend the afternoon on the build, the brief, and the video.

Three things to hand in:

1. Code repository, pushed to git and kept public. Don't make it private after the deadline; it stays readable for review. It must build from a clean checkout with `make` and produce a runnable executable.
2. Game brief, a written document (your repo `README.md` is the natural home) covering:
- what the game is and how it connects to Malaysia Day
- how to build and run it, including any library you need installed and its version
- which library you chose and why, and what you built yourself versus what the library gave you
- a short "where Week 1 shows up in this code" map: const correctness, ownership/RAII, class design, containers, any pattern, any algorithm, tests
- what you cut and what you'd do differently with more time
1. Demo video, 5 minutes. A screen recording of the game actually being played, with real gameplay on screen rather than slides. Walk through the core loop first, then take us into the code for the parts you're proud of or that fought you hardest. Ten minutes is a lot of room, you don't have to use all of it, but keep the video engaging for the judges.

Submit the repository link in the google form posted on Discord before 5:00 PM on day 12.
