#include <stdio.h>
#include <dryos.h>
#include <module.h>
#include <menu.h>
#include <config.h>
#include <console.h>

/* temporary, printf %c and putchar isn't working */
static void putch(int a)
{
    printf((char[]){a, '\0'});
}

static void cos_task()
{
    msleep(100);

    console_clear();
    console_show();

    char input[] = "!*********+++>|<.+>dd!<^>a!%*++^a!+^!?!^$|";

    int memtop[50] = {0};
    int membottom[1000] = {0};
    size_t topp = 0;
    size_t bottomp = 0;

    // Map the labels in the array
    int labels[50];
    int l = 0;
    for (int c = 0; input[c] != '\0'; c++)
    {
        if (input[c] == '|')
        {
            labels[l] = c;
            l++;
        }
    }

    for (int c = 0; input[c] != '\0'; c++)
    {
        switch (input[c]) {
            case '!':
                membottom[bottomp] = 0;
                break;
            case '%':
                membottom[bottomp] += 50;
                break;
            case '*':
                membottom[bottomp] += 5;
                break;
            case '+':
                membottom[bottomp]++;
                break;
            case '-':
                membottom[bottomp]--;
                break;
            case '.':
                putch(membottom[bottomp]);
                break;
            case '>':
                bottomp++;
                break;
            case '<':
                bottomp--;
                break;
            case 'd':
                topp++;
                break;
            case 'a':
                topp--;
                break;
            case '^':
                memtop[topp] = membottom[bottomp];
                break;
            case 'v':
                membottom[bottomp] = memtop[topp];
                break;
            case '$':
                c = labels[memtop[topp]];
                break;
            case '?':
                if (memtop[topp + 1] == memtop[topp + 2]) {
                    c = labels[memtop[topp]];
                }

                break;
        }
    }

    puts("\nProgram done.");
}

static struct menu_entry cos_menu[] =
{
    {
        .name       = "CrypticOS",
        .select     = run_in_separate_task,
        .priv       = cos_task,
        .help       = "Start the CrypticOS Emulator.",
    },
};

/* This function is called when the module loads. */
/* All the module init functions are called sequentially,
 * in alphabetical order. */
static unsigned int cos_init()
{
    menu_add("Debug", cos_menu, COUNT(cos_menu));
    return 0;
}

/* Note: module unloading is not yet supported;
 * this function is provided for future use.
 */
static unsigned int cos_deinit()
{
    return 0;
}

/* All modules have some metadata, specifying init/deinit functions,
 * config variables, event hooks, property handlers etc.
 */
MODULE_INFO_START()
    MODULE_INIT(cos_init)
    MODULE_DEINIT(cos_deinit)
MODULE_INFO_END()
