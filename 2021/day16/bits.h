#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum ExpressionNodeType {
    SUM,
    PRODUCT,
    MINIMUM,
    MAXIMUM,
    VALUE,
    GREATER_THAN,
    LESS_THAN,
    EQUAL_TO
};

enum OperatorLengthType {
    SUB_PACKET_LENGTH,
    SUB_PACKET_COUNT
};

struct ExpressionNode {
    enum ExpressionNodeType type;
    int version;
    long value;

    struct ExpressionNode **nodes;
    int nodeCount;
};

struct ExpressionNode *addNode(struct ExpressionNode *node) {
    if (node) {
        ++node->nodeCount;

        if (node->nodeCount == 0) {
            node->nodes = (struct ExpressionNode **)malloc(sizeof(struct ExpressionNode *));
        } else {
            node->nodes = (struct ExpressionNode **)realloc(node->nodes, node->nodeCount * sizeof(struct ExpressionNode *));
        }

        struct ExpressionNode *newNode = (struct ExpressionNode *)calloc(1, sizeof(struct ExpressionNode));

        node->nodes[node->nodeCount - 1] = newNode;

        return newNode;
    }

    return NULL;
}

void freeExpression(struct ExpressionNode *node) {
    for (int i = 0; i < node->nodeCount; i++) {
        freeExpression(node->nodes[i]);
    }

    free(node);
}

int versionSum(struct ExpressionNode *node) {
    int sum = node->version;

    for (int i = 0; i < node->nodeCount; i++) {
        sum += versionSum(node->nodes[i]);
    }

    return sum;
}

long evaluate(struct ExpressionNode *node) {
    long result;

    switch (node->type) {
        case SUM:
            result = 0;

            for (int i = 0; i < node->nodeCount; i++) {
                result += evaluate(node->nodes[i]);
            }
            break;
        case PRODUCT:
            result = 1;

            for (int i = 0; i < node->nodeCount; i++) {
                result *= evaluate(node->nodes[i]);
            }
            break;
        case MINIMUM:
            for (int i = 0; i < node->nodeCount; i++) {
                long value = evaluate(node->nodes[i]);

                if (i == 0 || value < result) {
                    result = value;
                }
            }
            break;
        case MAXIMUM:
            for (int i = 0; i < node->nodeCount; i++) {
                long value = evaluate(node->nodes[i]);

                if (i == 0 || value > result) {
                    result = value;
                }
            }
            break;
        case VALUE:
            result = node->value;
            break;
        case GREATER_THAN:
            result = evaluate(node->nodes[0]) > evaluate(node->nodes[1]) ? 1 : 0;
            break;
        case LESS_THAN:
            result = evaluate(node->nodes[0]) < evaluate(node->nodes[1]) ? 1 : 0;
            break;
        case EQUAL_TO:
            result = evaluate(node->nodes[0]) == evaluate(node->nodes[1]) ? 1 : 0;
            break;     
        default:
            break;       
    }

    return result;
}

void readPacket(char *packet, int *packetLength, struct ExpressionNode *node);

long toLong(char *bits) {
    long value = 0;

    long bitValue = 1L << (strlen(bits) - 1);
    char *bit = bits;

    while (*bit) {
        value += *bit == '1' ? bitValue : 0;

        bitValue >>= 1;
        ++bit;
    }

    return value;
}

void readValue(char *packet, int *packetLength, struct ExpressionNode *node) {
    char *valueNode = packet;
    char valueBits[65] = "";

    *packetLength += 5;

    strncat(valueBits, valueNode + 1, 4);

    while (*valueNode != '0') {
        valueNode += 5;
        *packetLength += 5;

        strncat(valueBits, valueNode + 1, 4);
    }

    node->value = toLong(valueBits);
}

void readOperator(char *packet, int *packetLength, struct ExpressionNode *node) {
    enum OperatorLengthType lengthType = *packet - '0';
    char subPacketInfoBits[16] = "";

    packet += 1;
    *packetLength += 1;

    switch (lengthType) {
        case SUB_PACKET_LENGTH:
            strncpy(subPacketInfoBits, packet, 15);

            long subPacketsLength = toLong(subPacketInfoBits);
        
            packet += 15;
            *packetLength += 15;

            int totalLength = 0;

            while (totalLength < subPacketsLength) {
                int subPacketlength = 0;

                struct ExpressionNode *operatorNode = addNode(node);

                readPacket(packet, &subPacketlength, operatorNode);
                packet += subPacketlength;

                totalLength += subPacketlength;
            }
        
            *packetLength += subPacketsLength;            
            break;
        case SUB_PACKET_COUNT:
            strncpy(subPacketInfoBits, packet, 11);

            long subPacketsCount = toLong(subPacketInfoBits);

            packet += 11;
            *packetLength += 11;

            for (int i = 0; i < subPacketsCount; i++) {
                int subPacketlength = 0;

                struct ExpressionNode *operatorNode = addNode(node);

                readPacket(packet, &subPacketlength, operatorNode);
    
                packet += subPacketlength;
                *packetLength += subPacketlength;
            }
            break;
    }
}

void readPacket(char *packet, int *packetLength, struct ExpressionNode *node) {
    char versionBits[4] = "";
    char typeBits[4] = "";
    int length = 0;

    if (strchr(packet, '1')) {
        strncpy(versionBits, packet, 3);
        packet += 3;

        strncpy(typeBits, packet, 3);
        packet += 3;

        if (packetLength) {
            *packetLength += 6;
        }

        node->version = toLong(versionBits);
        node->type = toLong(typeBits);

        switch (node->type) {
            case VALUE:
                readValue(packet, &length, node);

                if (packetLength) {
                    *packetLength += length;
                }
                break;
            default:
                readOperator(packet, &length, node);

                if (packetLength) {
                    *packetLength += length;
                }
                break;
        }
    }
}

char *getTransmission(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char hexBits[][5] = {
            "0000",
            "0001",
            "0010",
            "0011",
            "0100",
            "0101",
            "0110",
            "0111",
            "1000",
            "1001",
            "1010",
            "1011",
            "1100",
            "1101",
            "1110",
            "1111"
        };
        char hex;
        int transmissionLength = 32;
        char *transmission = (char *)malloc(transmissionLength * sizeof(char));

        strcpy(transmission, "");

        while (!feof(inputFile) && (hex = fgetc(inputFile))) {
            int hexBitsIndex = hex > '9' ? 10 + hex - 'A' : hex - '0';

            if (hexBitsIndex >=0 && hexBitsIndex < 16) {
                if (strlen(transmission) + 4 > transmissionLength) {
                    transmissionLength += 1024;

                    transmission = (char *)realloc(transmission, transmissionLength * sizeof(char));
                }

                strcat(transmission, hexBits[hexBitsIndex]);
            }
        }

        fclose(inputFile);

        return transmission;
    }

    return NULL;
}