#ifndef _MACROS_H
#define _MACROS_H

#define m_setFlag(byte, flag) byte |= (flag)
#define m_unsetFlag(byte, flag) byte &= ~(flag)
#define m_toggleFlag(byte, flag) byte ^= (flag)
#define m_isFlagSet(byte, flag) ((byte & (flag)) > 0)
#define m_setAnimation(entity, animId) \
    if (entity->animation != animId) { \
        entity->animation = animId;    \
        entity->frame = 0;             \
    }

// This works because sprite is same size as tile. If not, we would need to adjust the x position to center the sprite with the tile
#define m_snapXToGrid(entity) entity->x = (uint16_t)(entity->x + entity->sprite->width * 0.5) / TILE_SIZE * TILE_SIZE;

#endif