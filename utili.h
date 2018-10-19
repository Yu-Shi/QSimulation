#ifndef UTILI_H
#define UTILI_H

#define CONNECTOR_SIZE 10
#define PIPE_LENGTH 80
#define INTERVAL (CONNECTOR_SIZE + PIPE_LENGTH)
#define START_POS 100
#define DEFAULT_WIDTH 200

class Utili
{
private:
    Utili(); // forbidden
public:
    typedef enum { PIPE_VER, PIPE_HOR } dir_type;
    typedef enum { MODE_WIDTH_ADJUSTABLE, MODE_WIDTH_NOT_ADJUSTABLE } simulation_mode;
};

#endif // UTILI_H
