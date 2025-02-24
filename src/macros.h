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

#endif