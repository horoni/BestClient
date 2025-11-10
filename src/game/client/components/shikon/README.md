# Shikon
## Description
Hack based on [fluffytw](https://github.com/krxclient/fluffytw) hacking library

## TODO's
### Features
 - [x] Laser Aimbot
 - [x] AutoLaser (When enemy arrive to the screen he immediately will be shooted)
 - [x] ESP: Draw prediction
 - [ ] Auto hit through wall with hammer
 - [ ] Head balancer
 - [ ] Auto Unfreeze
 - [ ] Avoid Freeze (Legit mode: You cant walk or hook (if it on top, so you can fall) to freeze. Aggressive: you CANT FALL TO FREEZE at all, hooks everywhere and controls left-right to prevent fall to freeze)
 - [ ] Implement grenade prediction
 - [ ] Auto Switch to Laser if `AutoLaser` enabled
 - [ ] Coordinates aligner (Walk to center of tile)

### Improvements
 - [ ] ESP(Predict): Draw green and red dots like KRX

### Refactor
 - [ ] Maybe there is a better way to detect shotgun mode?

### Fixes
 - [ ] `HitScanWeapon()`: `IntersectLineTeleHook()` can it be used with any weapon?
 - [ ] Use Local Tuning instead Global
 - [ ] Fix weapon reach constants (Hammer!)
 - [ ] Fix ESP Draw FOV
 - [ ] Fix AutoLaser shoots earlier than enemy can be shooted (it shoots to wall instead enemy)

