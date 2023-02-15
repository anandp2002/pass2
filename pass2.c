#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    char opcode[10], operand[10], label[25], code[3], mnemonic[5], dash[10], locctr[10], addr[10],
        symb[10], symbaddr[10];
    int start, length, i = 0;
    FILE *fp1, *fp2, *fp3, *fp4, *fp5, *fp6;
    fp1 = fopen("optab.txt", "r");
    fp2 = fopen("symtab.txt", "r");
    fp3 = fopen("intermediate.txt", "r");
    fp4 = fopen("objectprogram.txt", "w");
    fp5 = fopen("length.txt", "r");
    fp6 = fopen("listing.txt", "w");
    fscanf(fp5, "%d", &length);
    fscanf(fp3, "\t\t%s\t%s\t%s", label, opcode, operand);
    int startAddress = atoi(operand);
    if (strcmp(opcode, "START") == 0)
    {
        fprintf(fp4, "H^%s^00%s^%d", label, operand, length);
        fprintf(fp6, "\t%s\t%s\t%s\n", label, opcode, operand);
        fscanf(fp3, "%s\t%s\t%s\t%s", locctr, label, opcode, operand);
    }
    fprintf(fp4, "\nT^");
    fprintf(fp4, "00%s", locctr);
    fscanf(fp2, "%s\t%s", symb, symbaddr);
    fscanf(fp1, "%s\t%s", mnemonic, code);
    while (strcmp(opcode, "END") != 0)
    {
        while (strcmp(mnemonic, "END") != 0)
        {
            if (strcmp(opcode, mnemonic) == 0)
            {
                while (strcmp(symb, "END") != 0)
                {
                    if (strcmp(operand, symb) == 0)
                    {
                        fprintf(fp6, "%s\t%s\t%s\t%s\t\t%s%s\n", locctr, label, opcode, operand, code, symbaddr);
                        fprintf(fp4, "^%s%s", code, symbaddr);
                        rewind(fp2);
                        break;
                    }
                    else if (strcmp(operand, "**") == 0)
                    {
                        fprintf(fp4, "^%s0000", code);
                        fprintf(fp6, "%s\t%s\t%s\t%s\t%s%s\n", locctr, label, opcode, operand, code, symbaddr);
                        rewind(fp2);
                        break;
                    }
                    fscanf(fp2, "%s\t%s", symb, symbaddr);
                }
                rewind(fp1);
                break;
            }
            fscanf(fp1, "%s\t%s", mnemonic, code);
        }
        if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0)
        {
            fprintf(fp6, "%s\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        }
        if (strcmp(opcode, "BYTE") == 0 || strcmp(opcode, "WORD") == 0)
        {
            if (strcmp(opcode, "BYTE") == 0)
            {
                fprintf(fp4, "^");
                fprintf(fp6, "\n%s\t%s\t%s\t%s\t", locctr, label, opcode, operand);
                for (int i = 2; i < strlen(operand) - 1; i++)
                {
                    fprintf(fp4, "%X", operand[i]);
                    fprintf(fp6, "%X", operand[i]);
                }
                fprintf(fp6, "\n");
            }
            if (strcmp(opcode, "WORD") == 0)
            {
                fprintf(fp6, "%s\t%s\t%s\t%s\t\t00000%s\n", locctr, label, opcode, operand, operand);
                fprintf(fp4, "^00000%s", operand);
            }
        }
        fscanf(fp3, "%s\t%s\t%s\t%s", locctr, label, opcode, operand);
    }
    fprintf(fp4, "\nE^%d", startAddress);
    fprintf(fp6, "%s\t%s\t%s\t%s", locctr, label, opcode, operand);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    fclose(fp6);
    return 0;
}