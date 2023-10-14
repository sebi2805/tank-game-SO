#ifndef PROJECTILE_MODEL_H
#define PROJECTILE_MODEL_H

class Projectile
{
public:
          Projectile(int tankId, int x, int y, char direction, int color);

          // Getter functions
          int getTankId() const;
          int getColor() const;
          int getX() const;
          int getY() const;
          char getDirection() const;

          // Setter functions
          void setTankId(int tankId);
          void setColor(int color);
          void setX(int x);
          void setY(int y);
          void setDirection(char direction);

          void move();

private:
          int tankId;
          int color;
          int x, y;
          char direction;
};

#endif