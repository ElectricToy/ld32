Ludum Dare 32
-------------

An Unconventional Weapon

# Goals

1. Fun
2. Easy, straightforward, familiar controls and concepts.
3. Create solid art/good visuals
4. Include sound and music

# Ideas

A weapon is an instrument of damage, destruction, or at least of opposition. Therefore direct conflict will be important here. I'm not keen to build a whole side scroller or whatever to feature conflict. So there's a tension already: interesting weapons call for interesting environments and/or enemies; yet the interesting environments and/or enemies will tend to overwhelm both development and gameplay. So: how to design a game that has a familiar quality, and features combat, yet is simple for the sake of development *and* to highlight the weapon?

One approach is to offer a straightforward game genre but give the player a weapon he's never had before in that context.

A big part of me would like to avoid that whole shebang though by doing something more unusual, more Papers Please-like.

Rather than brainstorming the genre first I'm going to start by brainstorming weapons.

## Unconventional Weapons

+ Magnetizer magnetizes enemies, causing them to stick to each other and environment
0 Love letter
0 Heals undead
0 Anti-grav
0 Wolves are you weapons, which you command (think bugs in HL2)
0 Poison
0 Light is weapon against darkness monsters
0 Light is weapon against oozing darkness
0 Creates time weirdness--pockets of slow time, reversed time, etc.
0 Sucks in bullets rather than firing them
- Brave New World weapon: propaganda keeps people complacent
    - "Elements" are ordinary citizens
    - Actions are inserting words into news reports, new products
    - Events cause downturns in happiness, you must distract
    - Ultimate downturn is a revolt/revolution
    - I think this is just too hard to make, art and all, in 24 hours.
- Weapon is "meta"--deals with levels of reality
- Ivy/plan weapon tangles things up
- Creates walls
- Destroys planets
- A person is the weapon
- Sword can slice through anything--even terrain/walls
- Slime weapon
- Feeds people groceries so they get fat and die
- Water attacks fire creatures
- Electric whip
- Fire vs. plants and trees
- Frying pan
- Water attacks slime
- Lightning is your weapon
- Tesla effect
- Venom
- Ice/cold is the weapon
- Combo weapon: two parts are harmless but together they're nasty.

Core Wars

## Observations

I'm attracted to the non-weapon weapons, like words and letters and propaganda. You have only to look at existing games to see that electricity/lightning, poison, slime, fire, water, acid, ice, etc. are all perfectly conventional.

I'm undecided on the weapon. Let's think about genre.

Let's focus on a Xevious-style scroller with a tap-only interface. What can we do with that that emphasizes skill and pizzazz?

## Genre Brainstorm

Xevious-style scroller - but could we maybe make this a single-click/tap affair?
Asteroids- or Time Pilot 1984 scroller
Isometric
Geometric/structural
Papers Please-style fixed screen - nervous about art budget here
Text
Platformer

## DECISION: Xevious-style programmer: Drone

Title: Drone
Short: Drones are the new, UNCONVENTIONAL weapons that are being deployed to defend earth from invading aliens. Unfortunately they haven't been programmed yet. Get programming!

Top half of screen is the drone vs. aliens in a steady scroller.

Bottom half (more like 1/3rd really) is a text programming environment. Programming model is extremely simplified but reasonably robust. Or I could just import a JavaScript interpreter or something....

# Schedule

8		Ideation
9		Prototype
10
11
12
1
2
3
4		Sleep
5		
6
7
8
9		Production (art, music, sound, levels)
10		
11
12
1
2
3
4
5
6
7
8		
9		Refinement
10
11
12		
1		Sleep
2
3
4
5
6
7
8		Publication

# Design

Landscape is one big landscape in the Xevious style, more or less.
Enemies (which have shadows), are standard Xevious/Galaxian-style alien robot affairs. But there are also cowering Humans and Human vehicles on the ground.

The player ship is at the bottom of the screen with an upward-facing machine guns and an occasional missile. It can only move left and right.

SORTIE

The player is basically programming the AI for the thing.

A reasonably robust internal file/copy and paste mechanism will be needed. Copy and paste from the OS would be nice but not absolutely necessary.

At the very bottom of the screen are a few rows:

*   sensors (nearest enemy/bogie, missiles remaining, etc.)
*   controls (left, right, fire)
*   functions, EXCEL-style

Hm, not so sure about this. Seems like a lot of work. Maybe just a Javascript terminal? But then the difficulty comes in plugging in an interpreter and in teaching the player.

In fact, now I think about it, overall this just smacks of violating my #1 goal, which is fun, and #2 goal, which is easy, straightforward and familiar concepts and controls. Sheesh.

