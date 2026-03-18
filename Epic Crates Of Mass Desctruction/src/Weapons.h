#ifndef WEAPONS_H
#define WEAPONS_H

#define POWERUP_MAXTIME     600
enum WEAPON_TYPE { DEFAULT_GUN = 0, SHOTGUN, MAGNUM, MACHINEGUN, FLAMETHROWER, DUAL_PISTOLS};
WEAPON_TYPE CURRENT_WEAPON;

class Bullet
{
  public:
    uint8_t x;
    uint8_t y;
    uint8_t width = 4;
    uint8_t height = 1;
    uint8_t fuse = 0;
    uint8_t xSpeed = 3;
    uint8_t ySpeed = 0;
    uint8_t damage = 1;
    boolean isActive = false;

    void Update() {
      if (isActive) {
        fuse--; if (fuse == 0) isActive = false;
        x += xSpeed;
        y += ySpeed;
      }
    }
};

class Weapons
{
  public:

    static const uint8_t maxBullets = 6;
    int powerupTimer = POWERUP_MAXTIME;
    uint8_t maxFuse = 23;
    uint8_t fuse = 0;
    uint8_t frameDelay = 1;
    uint8_t maxCooldown =  3;
    uint8_t kickBack = 0;
    uint8_t cooldown = 0;
    uint8_t currentBullet = 0;
    uint8_t width = 4;
    uint8_t height = 1;
    uint8_t xSpeed = 3;
    uint8_t xSpeedOffset = 0;
    uint8_t yOffset = 0;
    uint8_t ySpeed = 0;
    uint8_t ySpeedOffset = 0;
    uint8_t damage = 1;

    Bullet bullet[maxBullets];

    void shootDual()
    {
      currentBullet ++; if (currentBullet == maxBullets)currentBullet = 0;
      bullet[currentBullet].isActive = true;
      bullet[currentBullet].fuse =  maxFuse ;
      bullet[currentBullet].ySpeed = random(0, ySpeedOffset + 1);
      uint8_t rand = random(0, 2); if (rand == 1) bullet[currentBullet].ySpeed *= -1;
    }
    boolean shoot()
    {
      if (cooldown == 0)
      {
        currentBullet ++; if (currentBullet == maxBullets)currentBullet = 0;
        bullet[currentBullet].isActive = true;
        bullet[currentBullet].fuse =  maxFuse ;
        bullet[currentBullet].ySpeed = random(0, ySpeedOffset + 1);
        uint8_t rand = random(0, 2); if (rand == 1) bullet[currentBullet].ySpeed *= -1;

        cooldown = maxCooldown;
        return true;
      }
      else
      {
        return false;
      }
    }


    void Update()
    {

      //timer for weapons powerup (to avoid exploitation)
      powerupTimer --;
      
      if (cooldown > 0) cooldown --;

      for (uint8_t i = 0; i < maxBullets; i++)
      {
        if (bullet[i].isActive)
        {
          bullet[i].Update();
        }
      }
    }

    void setBulletSpecs()
    {
      for (uint8_t i = 0; i < maxBullets; i++)
      {
        bullet[i].width = width;
        bullet[i].height = height;
        bullet[i].xSpeed = xSpeed;
        bullet[i].ySpeed = ySpeed;
        bullet[i].damage = damage;
        bullet[i].isActive = false;
      }

    }

    void weaponSet(int weapType)
    {
      powerupTimer = POWERUP_MAXTIME;
        switch (weapType) {
        case 0: //DEFAULT GUN
          frameDelay = 1;
          maxFuse = 30;
          fuse = 0;
          maxCooldown =  3;
          kickBack = 0;
          cooldown = 0;
          width = 4;
          height = 2;
          xSpeed = 3;
          xSpeedOffset = 0;
          yOffset = 0;
          ySpeed = 0;
          ySpeedOffset = 0;
          damage = 1;
          break;
        case 1: //SHOTGUN
          frameDelay = 1;
          maxFuse = 16;
          fuse = 0;
          maxCooldown =  40;
          kickBack = 8;
          cooldown = 0;
          width = 16;
          height = 8;
          xSpeed = 1;
          ySpeed = 0;
          yOffset = 0;
          ySpeedOffset = 0;
          damage = 5;
          break;
        case 2: //MAGNUM
          frameDelay = 1;
          maxFuse = 35;
          fuse = 0;
          maxCooldown =  20;
          kickBack = 4;
          cooldown = 0;
          width = 5;
          height = 3;
          xSpeed = 3;
          xSpeedOffset = 0;
          ySpeed = 0;
          yOffset = 0;
          ySpeedOffset = 0;
          damage = 2;
          break;
        case 3: //MACHINE_GUN:
          frameDelay = 1;
          maxFuse = 30;
          fuse = 0;
          maxCooldown =  8;
          kickBack = 5;
          cooldown = 0;
          width = 5;
          height = 3;
          xSpeed = 4;
          xSpeedOffset = 0;
          ySpeed = 0;
          yOffset = 1;
          ySpeedOffset = 0;
          damage = 1;
          break;
        case 4: //FLAMETHROWER:
          frameDelay = 3;
          maxFuse = 6;
          fuse = 0;
          maxCooldown =  4;
          cooldown = 0;
          kickBack = 0;
          width = 6;
          height = 6;
          xSpeed = 3;
          xSpeedOffset = 2;
          ySpeed = 0;
          yOffset = 1;
          ySpeedOffset = 1;
          damage = 2;
          break;
        case 5: //DUAL_PISTOLS:
          //SAME STATS AS DEFAULT_GUN
          frameDelay = 1;
          maxFuse = 30;
          fuse = 0;
          maxCooldown = 3;
          kickBack = 0;
          cooldown = 0;
          width = 4;
          height = 2;
          xSpeed = 3;
          xSpeedOffset = 0;
          yOffset = 0;
          ySpeed = 0;
          ySpeedOffset = 0;
          damage = 1;
          break;
      }
      setBulletSpecs();
    }
};

Weapons weapon;

void makeShootingSounds() //pew pew sound
{
  switch (CURRENT_WEAPON)
  {
    case DEFAULT_GUN:
      sound.tone(100, 100);
      break;
    case SHOTGUN:
      sound.tone(140, 300);
      break;
    case MAGNUM:
      sound.tone(190, 300);
      break;
    case MACHINEGUN:
      sound.tone(120, 100);
      break;
    case FLAMETHROWER:
      sound.tone(100, 80);
      break;
    case DUAL_PISTOLS:
      sound.tone(100, 100);
      break;
  }
}

void shoot()
{
  if ((arduboy.pressed(A_BUTTON) && CURRENT_WEAPON == MACHINEGUN)  || (arduboy.pressed(A_BUTTON) && CURRENT_WEAPON == FLAMETHROWER) || arduboy.justPressed(A_BUTTON))
  {
    //shoot normal bullets
    if (weapon.shoot()) {
      makeShootingSounds();
      uint8_t yOffset = weapon.yOffset;
      uint8_t randNum = random(0, 2); if (randNum == 1) yOffset *= -1;
      weapon.bullet[weapon.currentBullet].y = player.y + 2 + yOffset ;
      uint8_t randxSpeedOffset = random(1, weapon.xSpeedOffset + 1);
      weapon.bullet[weapon.currentBullet].x = player.x + ((1 - (2 * playerFacing)) * 8);
      weapon.bullet[weapon.currentBullet].xSpeed =  ((1 - (2 * playerFacing)) * weapon.xSpeed) - ((1 - (2 * playerFacing)) * randxSpeedOffset);
      {
        for (uint8_t i = 0; i <  weapon.kickBack; i++)
        {
          if (!checkCollisions()) player.x -= (1 - (2 * playerFacing));
        }
      }
      if (CURRENT_WEAPON == DUAL_PISTOLS)
      {
        weapon.shootDual();
        weapon.bullet[weapon.currentBullet].y = player.y + 3;
        weapon.bullet[weapon.currentBullet].x = player.x - ((-1 * playerFacing) * weapon.width);
        weapon.bullet[weapon.currentBullet].xSpeed = -  ((1 - (2 * playerFacing)) * weapon.xSpeed) + ((1 - (2 * playerFacing)));
      }
    }
  }
}

void updateWeapon()
{
  if (arduboy.everyXFrames(weapon.frameDelay))
  {
    weapon.Update();
    if (weapon.powerupTimer < 1)
    {
      CURRENT_WEAPON = DEFAULT_GUN;
      weapon.weaponSet(0);
    }
  }
}

void drawBullets()
{
  for (uint8_t i = 0; i < weapon.maxBullets; i++)
  {
    if (weapon.bullet[i].isActive)  sprites.drawSelfMasked(weapon.bullet[i].x, weapon.bullet[i].y, bullets_is_mask, CURRENT_WEAPON + (playerFacing * 6));
  }
}

#endif
