#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_PERSONAGENS 5
#define VIDA_INICIAL 100

typedef enum 
{
    GUERREIRO, MAGO, CACADOR, PALADINO, BARBARO
} Classe;

const char *nomes_classes[] = 
{
    "Guerreiro", "Mago", "Caçador", "Paladino", "Bárbaro"
};

typedef struct 
{
    Classe classe;
    int vida;
    int ataque;
    int defesa;
    int habilidade_ativa;
} Personagem;

void ini_pers(Personagem *p, Classe classe);
void ini_time(Personagem time[]);
void imp_est_times(Personagem time1[], Personagem time2[]);
int todos_mortos(Personagem time[]);
int esc_atac(Personagem time[]);
int esc_alvo(Personagem time[]);
void real_atac(Personagem *atacante, Personagem *defensor);
float gerar_chance();

int main() 
{
    srand(time(NULL));
    
    int opcao;
    do {
        printf("===== SIMULADOR DE COMBATE POR TURNOS =====\n");
        printf("1 - Nova Simulação\n");
        printf("2 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) 
        {
            Personagem time1[NUM_PERSONAGENS];
            Personagem time2[NUM_PERSONAGENS];

            ini_time(time1);
            ini_time(time2);

            int time_da_vez = rand() % 2;
            printf("\n>>> O Time %d começa a batalha!\n", time_da_vez + 1);

            int rodada = 1;

            while (!todos_mortos(time1) && !todos_mortos(time2)) 
            {
                printf("\n>>> Rodada %d:\n", rodada);

                Personagem *time_atacante = (time_da_vez == 0) ? time1 : time2;
                Personagem *time_defensor = (time_da_vez == 0) ? time2 : time1;

                int idx_atacante = esc_atac(time_atacante);
                int idx_defensor = esc_alvo(time_defensor);

                printf("Time %d — %s (Vida: %d) ataca %s do Time %d (Vida: %d)\n",
                    time_da_vez + 1,
                    nomes_classes[time_atacante[idx_atacante].classe],
                    time_atacante[idx_atacante].vida,
                    nomes_classes[time_defensor[idx_defensor].classe],
                    2 - time_da_vez,
                    time_defensor[idx_defensor].vida);

                real_atac(&time_atacante[idx_atacante], &time_defensor[idx_defensor]);

                time_da_vez = 1 - time_da_vez;
                rodada++;

                imp_est_times(time1, time2);
            }

            printf(">>> FIM DO COMBATE\n");
            if (todos_mortos(time1) && todos_mortos(time2)) 
            {
                printf(">>> EMPATE!\n");
            } 
            else if (todos_mortos(time2)) 
            {
                printf(">>> TIME 1 VENCEU!\n");
            } 
            else 
            {
                printf(">>> TIME 2 VENCEU!\n");
            }
            printf("\n");

            printf("Pressione ENTER para continuar...");
            getchar(); getchar();
        } 
        else if (opcao == 2) 
        {
            printf("Encerrando o programa...\n");
        } 
        else 
        {
            printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 2);

    return 0;
}

void ini_pers(Personagem *p, Classe classe) 
{
    p->classe = classe;
    p->vida = VIDA_INICIAL;
    p->habilidade_ativa = 0;

    switch (classe) 
    {
        case GUERREIRO: p->ataque = 20; p->defesa = 10; break;
        case MAGO:      p->ataque = 30; p->defesa = 5;  break;
        case CACADOR:   p->ataque = 18; p->defesa = 8;  break;
        case PALADINO:  p->ataque = 15; p->defesa = 12; break;
        case BARBARO:   p->ataque = 25; p->defesa = 6;  break;
    }
}

void ini_time(Personagem time[]) 
{
    for (int i = 0; i < NUM_PERSONAGENS; i++) 
    {
        ini_pers(&time[i], (Classe)i);
    }
}

void imp_est_times(Personagem time1[], Personagem time2[]) 
{
    printf("\n>>> Estado Atual:\n");

    printf("Time 1:\n");
    for (int i = 0; i < NUM_PERSONAGENS; i++) 
    {
        printf("  %s: Vida %d\n", nomes_classes[time1[i].classe], time1[i].vida);
    }

    printf("\nTime 2:\n");
    for (int i = 0; i < NUM_PERSONAGENS; i++) 
    {
        printf("  %s: Vida %d\n", nomes_classes[time2[i].classe], time2[i].vida);
    }
    printf("\n");
}

int todos_mortos(Personagem time[]) 
{
    for (int i = 0; i < NUM_PERSONAGENS; i++) 
    {
        if (time[i].vida > 0) return 0;
    }
    return 1;
}

int esc_atac(Personagem time[]) 
{
    float melhor_razao = -1;
    int idx = -1;
    for (int i = 0; i < NUM_PERSONAGENS; i++) 
    {
        if (time[i].vida > 0) 
        {
            float razao = (float)time[i].vida / time[i].ataque;
            if (razao > melhor_razao) 
            {
                melhor_razao = razao;
                idx = i;
            }
        }
    }
    return idx;
}

int esc_alvo(Personagem time[]) 
{
    int vivos[NUM_PERSONAGENS], count = 0;
    for (int i = 0; i < NUM_PERSONAGENS; i++) 
    {
        if (time[i].vida > 0) vivos[count++] = i;
    }
    if (count == 0) return -1;
    return vivos[rand() % count];
}

float gerar_chance() 
{
    return (float)rand() / RAND_MAX;
}

void real_atac(Personagem *atacante, Personagem *defensor) 
{
    int dano = 0;
    int ataque_erro = (gerar_chance() < 0.2);
    int defesa_falha = (gerar_chance() < 0.2);

    if (ataque_erro && atacante->classe != BARBARO) 
    {
        printf("→ Ataque errou! Nenhum dano aplicado.\n");
        return;
    }

    int defesa = (defesa_falha) ? 0 : defensor->defesa;

    dano = atacante->ataque - defesa;
    if (dano < 0) dano = 0;

    switch (atacante->classe) 
    {
        case GUERREIRO:
            if (gerar_chance() < 0.2) 
            {
                dano *= 2;
                printf("→ Habilidade especial do Guerreiro ativada: Golpe Crítico!\n");
            }
            break;
        case MAGO:
            if (gerar_chance() < 0.25) 
            {
                dano = atacante->ataque;
                printf("→ Habilidade especial do Mago ativada: Bola de Fogo (ignora defesa)!\n");
            }
            break;
        case CACADOR:
            if (gerar_chance() < 0.15) 
            {
                printf("→ Habilidade especial do Caçador ativada: Ataque Duplo!\n");
                real_atac(atacante, defensor);
                return;
            }
            break;
        default:
            break;
    }

    defensor->vida -= dano;
    if (defensor->vida < 0) defensor->vida = 0;

    printf("→ Dano aplicado: %d\n", dano);

    if (defensor->classe == PALADINO && defensor->vida > 0 && gerar_chance() < 0.3) 
    {
        int vida_perdida = VIDA_INICIAL - defensor->vida;
        int cura = vida_perdida * 0.2;
        defensor->vida += cura;

        if (defensor->vida > VIDA_INICIAL)
            defensor->vida = VIDA_INICIAL;
        printf("→ Habilidade especial do Paladino ativada: Regeneração de %d de vida!\n", cura);
    }
}