int moveScore(char move, char rock) {
    return move - rock + 1;
}

char winningMove(char move, char rock) {
    switch (move) {
    case 'A':
        return rock + 1;
    case 'B':
        return rock + 2;
    case 'C':
        return rock;
    }

    return move;
}

char sameMove(char move, char rock) {
    switch (move) {
    case 'A':
        return rock;
    case 'B':
        return rock + 1;
    case 'C':
        return rock + 2;
    }

    return move;
}

char losingMove(char move, char rock) {
    switch (move) {
    case 'A':
        return rock + 2;
    case 'B':
        return rock;
    case 'C':
        return rock + 1;
    }

    return move;
}
