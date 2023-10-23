#ifndef LABELS_H
#define LABELS_H

const int POISON_LABEL = -1;
const int MAX_SIZE_LABEL = 100;
struct Label {
    const char *name;
    int index;
};
const int MAX_COUNT_LABELS = 50;

#endif // LABELS_H