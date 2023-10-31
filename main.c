#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#pragma pack(2)




typedef struct
{

    char BitType[2];
    uint32_t BitSize; //tamanho do arquivo em Bytes
    uint16_t BfReser1;
    uint16_t BfReser2;
    uint32_t BfOffSetBit;

    uint32_t BiSize; //tamanho do cabecalho 40bytes
    uint32_t BiWidth;
    uint32_t BiHeight;
    uint16_t BiPlanes;
    uint16_t BiBitCount;
    uint32_t BiCompress;
    uint32_t BiSizeImag;
    uint32_t BiXPPMeter;
    uint32_t BiYPPMeter;
    uint32_t BiClrUsed;
    uint32_t BiClrImpor;

} BMPinfoHead;
#pragma pack(pop)




int main()
{

    //variaveis para criar varios arquivos
    char arquivo[40];
    char arquivo_GR[40];
    char arquivo_g[40];
    char arquivo_r[40];
    char arquivo_b[40];
    char arquivo_INV[40];
    char arquivo_CORT[40];

    int i;
    int j;




    printf("Qual o nome do arquivo\n");
    gets(arquivo);

    strcpy(arquivo_GR, arquivo);
    strcpy(arquivo_g, arquivo);
    strcpy(arquivo_r, arquivo);
    strcpy(arquivo_b, arquivo);
    strcpy(arquivo_INV, arquivo);
    strcpy(arquivo_CORT, arquivo);

    strcat(arquivo_GR, "_gs.bmp");
    strcat(arquivo_g, "_g.bmp");
    strcat(arquivo_r, "_r.bmp");
    strcat(arquivo_b, "_b.bmp");
    strcat(arquivo_INV, "_inv.bmp");
    strcat(arquivo_CORT, "_cort.h");



    BMPinfoHead Head;






    FILE *fzao = fopen("paraguaio.bmp", "rb");
    FILE *azul = fopen(arquivo_b, "wb");
    FILE *vermelho = fopen(arquivo_r, "wb");
    FILE *verde = fopen(arquivo_g, "wb");
    FILE *cinza = fopen(arquivo_GR, "wb");
    FILE *invertido = fopen(arquivo_INV, "wb");
    FILE *cortado = fopen(arquivo_CORT, "wb");
    int d;



    if(!fzao)
    {
        printf("Erro ao abrir o arquivo BMP.\n");
        return 1;
    }


    fread(&Head, sizeof(BMPinfoHead), 1, fzao);

    if(Head.BitType[0]=='B' && Head.BitType[1] == 'M')
    {
        printf("primeiros characteres %c%c\n\n", Head.BitType[0], Head.BitType[1]);
    }
    else
    {
        printf("Não é um arquivo BMP amigo\n");
    }



    fwrite(&Head, sizeof(BMPinfoHead), 1, azul);

    printf("Nome do arquivo: %s \n", arquivo);
    printf("Tamanho: %dx%d pixels\n", Head.BiHeight, Head.BiWidth);



    int width = abs(Head.BiWidth);
    int height = abs(Head.BiHeight);
    int linha = ((width * Head.BiBitCount + 31) / 32) * 4;//linha em bytes

    // Criar um local na memoria para armazenar os dados de pixel
    char *pixelData_azul = (char *)malloc(linha * height);//Função malloc, precisa saber para onde ira apontar.

    // Ler os dados de pixel
    fread(pixelData_azul, 1, linha * height, fzao);

    //arquivo todo azul
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            //variavel pra pegar os bits
            int offset = i * linha + j * (Head.BiBitCount / 8);


            //pixelData_azul[offset]// Azul
            pixelData_azul[offset+1] = 0; // Verde
            pixelData_azul[offset + 2] = 0; // Vermelho
        }
    }

    // Escrever os dados de pixel modificados no arquivo de saída
    fwrite(pixelData_azul, 1, linha * height, azul);





    rewind(fzao);

    //arquivo todo verde
    fread(&Head, sizeof(BMPinfoHead), 1, fzao);
    fwrite(&Head, sizeof(BMPinfoHead), 1, verde);

    char *pixelData_verde = (char *)malloc(linha * height);

    fread(pixelData_verde, 1, linha * height, fzao);



    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //variavel pra pegar os bits
            int offset = i * linha + j * (Head.BiBitCount / 8);


            pixelData_verde[offset] = 0;// Azul
            //pixelData_verde[offset+1]// Verde
            pixelData_verde[offset + 2] = 0; // Vermelho
        }
    }

    fwrite(pixelData_verde, 1, linha * height, verde);








    rewind(fzao);

    //arquivo todo vermelho
    fread(&Head, sizeof(BMPinfoHead), 1, fzao);
    fwrite(&Head, sizeof(BMPinfoHead), 1, vermelho);

    char *pixelData_vermelho = (char *)malloc(linha * height);
    fread(pixelData_vermelho, 1, linha * height, fzao);

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            //variavel pra pegar os bits
            int offset = i * linha + j * (Head.BiBitCount / 8);

            pixelData_vermelho[offset] = 0;// Azul
            pixelData_vermelho[offset+1] = 0; // Verde
            //pixelData_vermelho[offset + 2]; // Vermelho
        }
    }

    // Escrever os dados de pixel modificados no arquivo de saída
    fwrite(pixelData_vermelho, 1, linha * height, vermelho);






    rewind(fzao);

    fread(&Head, sizeof(BMPinfoHead), 1, fzao);
    fwrite(&Head, sizeof(BMPinfoHead), 1, cinza);

    char *pixelData_cinza = (char *)malloc(linha * height);
    fread(pixelData_cinza, 1, linha * height, fzao);


    // Converter os tons de cores em tons de azul
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            //variavel pra pegar o pixel
            int offset = i*linha + j*(Head.BiBitCount / 8);

            uint8_t verdi = 0, vermei = 0, azur = 0;
            vermei = pixelData_cinza[offset];
            azur = pixelData_cinza[offset+1];
            verdi = pixelData_cinza[offset+2];



            uint8_t GR = (0.11*azur+0.59*verdi+0.3*vermei);


            //Transformando tudo em cinza
            pixelData_cinza[offset] = GR;// verde
            pixelData_cinza[offset+1]= GR; // vermelho
            pixelData_cinza[offset+2] = GR; // azul
        }
    }

    // Escrever os dados de pixel modificados no arquivo de saída
    fwrite(pixelData_cinza, 1, linha * height, cinza);















    rewind(fzao);

    fread(&Head, sizeof(BMPinfoHead), 1, fzao);
    fwrite(&Head, sizeof(BMPinfoHead), 1, invertido);

    char *pixelData_invertido = (char *)malloc(linha * height);
    fread(pixelData_invertido, 1, linha * height, fzao);


    // Converter os tons de cores em tons de azul
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            //variavel pra pegar os bits
            int offset = i * linha + j * (Head.BiBitCount / 8);
            int aux = 0;
            int aux2 = 0;


            //invertendo
            aux = pixelData_invertido[offset];//pega azul
            pixelData_invertido[offset] = pixelData_invertido[offset + 1];// Azul vira vermelho
            aux2 = pixelData_invertido[offset+2];//pega verde
            pixelData_invertido[offset+2] = aux;// Verde vira azul
            pixelData_invertido[offset + 1] = aux2; // Vermelho vira verde
        }
    }

    // Escrever os dados de pixel modificados no arquivo de saída
    fwrite(pixelData_invertido, 1, linha * height, invertido);





rewind(fzao);



    fread(&Head, sizeof(BMPinfoHead), 1, fzao);
    Head.BiWidth = 84;
    Head.BiHeight = 48;
    Head.BiSizeImag = 84 * 48 * 3;
    Head.BitSize = Head.BfOffSetBit + 84 * 48 * 3;
    fwrite(&Head, sizeof(BMPinfoHead), 1, cortado);




    uint8_t coordX, coordY, limiar;
    printf("quais as coordenadas X e Y parceiro. \n\n");
    printf("\n");
    scanf("%i", &coordX);
    scanf("%i", &coordY);
    printf("qual o valor do limiair parceiro. \n\n");
    scanf("%i", &limiar);
    printf("\n");





    char *pixelData_cortado = (char *)malloc(84 * 48 * 3);


    // Converter os tons de cores em tons de azul
    for (i = 0; i < 48; i++)
    {
        for (j = 0; j < 84; j++)
        {
            //variavel pra pegar o pixel
            int src_offset = (i + coordY) * linha + (j + coordX) * 3;
            int dest_offset = i * 84 * 3 + j * 3;


            pixelData_cortado[dest_offset] = pixelData_cinza[src_offset];             // Azul
            pixelData_cortado[dest_offset + 1] = pixelData_cinza[src_offset + 1];     // Verde
            pixelData_cortado[dest_offset + 2] = pixelData_cinza[src_offset + 2];



            uint8_t verdi = 0, vermei = 0, azur = 0;
            vermei = pixelData_cortado[dest_offset];
            azur = pixelData_cortado[dest_offset+1];
            verdi = pixelData_cortado[dest_offset+2];



            uint8_t GR = (0.11*azur+0.59*verdi+0.3*vermei);

             int BW;
            //transformando em branco
            if(GR>limiar)
            {
                BW = 255;
            }
            else
            {
                BW=0;
            }



            pixelData_cortado[dest_offset] = BW;             // Azul
            pixelData_cortado[dest_offset + 1] = BW;     // Verde
            pixelData_cortado[dest_offset + 2] = BW;     // Vermelho



        }
    }

    // Escrever os dados de pixel modificados no arquivo de saída
    fwrite(pixelData_cortado, 1, 84 * 48*3, cortado);














    fclose(verde);
    fclose(vermelho);
    fclose(azul);
    fclose(fzao);
    fclose(cinza);
    fclose(invertido);
    fclose(cortado);



    return 0;
}




