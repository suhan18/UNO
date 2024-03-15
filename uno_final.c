#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct UNO_CARD
{
    int num;     // draw 2 card represented by 10;
    char colour; // R, G, B, Y
} card;
typedef struct sll
{
    card pCard;
    struct sll *next;

} p_deck; // player deck card

// 1) making individual color stacks - each with 0-9 - array implementation of stack
void populateColorStk(int colour_code, int *p2_top, card **p2_colorStack)
{
    char fill_colour;
    switch (colour_code)
    {
    case 1:
        fill_colour = 'R';
        break;

    case 2:
        fill_colour = 'B';
        break;

    case 3:
        fill_colour = 'G';
        break;

    case 4:
        fill_colour = 'Y';
        break;
    };

    *p2_colorStack = (card *)malloc(22 * sizeof(card));

    (*p2_top)++; // top is now standing at 0th index -> waiting for appending
    while (*p2_top < 22)
    {
        int cardNumber = *p2_top / 2;
        for (int i = 0; i < 2; i++)
        {
            (*p2_colorStack)[*p2_top].num = cardNumber;
            (*p2_colorStack)[*p2_top].colour = fill_colour;
            (*p2_top)++;
        }
    }
    (*p2_top)--; // coz top would have become 22 -> inaccessible in the array.
}

// 2) shuffling any deck that is passed to it
void shuffle(card deck[], int len)
{
    int max_index = len - 1;
    for (int i = max_index; i >= 0; i--) // shuffling from the top of the deck
    {
        // selecting a random index of the deck with which i have to swap the current card with
        int swap_with_index = rand() % (i + 1); // putting only i causes problem coz when i = 0; % operator undefined

        // swapping here
        card temp = deck[i];
        deck[i] = deck[swap_with_index];
        deck[swap_with_index] = temp;
    }
}

// 3) after picking a card from a color stack, it reduces the stack size and the top value
void reduceStackSize(card **p2_s, int *p2_t)
{
    if (*p2_t > -1)
    {
        *p2_s = realloc(*p2_s, sizeof(card) * (*p2_t));
        (*p2_t)--;
    }
}

// 4) this is creating the pile from which we distribute cards to players and draw from
// colour_codes :                   1                       2                       3                         4
void create_semi_Shuffled_Pile(card **p2_s1, int *p2_t1, card **p2_s2, int *p2_t2, card **p2_s3, int *p2_t3, card **p2_s4, int *p2_t4, card **p2_pile, int *p2_piletop)
{
    *p2_pile = (card *)malloc(sizeof(card) * 88);

    for (int i = 0; i < 22; i++)
    {
        // picking topmost card from each stack
        (*p2_pile)[++(*p2_piletop)] = (*p2_s1)[*p2_t1];
        reduceStackSize(p2_s1, p2_t1);

        (*p2_pile)[++(*p2_piletop)] = (*p2_s2)[*p2_t2];
        reduceStackSize(p2_s2, p2_t2);

        (*p2_pile)[++(*p2_piletop)] = (*p2_s3)[*p2_t3];
        reduceStackSize(p2_s3, p2_t3);

        (*p2_pile)[++(*p2_piletop)] = (*p2_s4)[*p2_t4];
        reduceStackSize(p2_s4, p2_t4);
    }
}

// 5)to pop a card from the pile of cards
card pop(int *p2_piletop, card **p2_pile)
{
    card return_val = (*p2_pile)[*p2_piletop];
    reduceStackSize(p2_pile, p2_piletop);
    return return_val;
}

// 6)inserting a card at the end of the stack

void insert_end(p_deck **head, card val)
{
    p_deck *newnode = (p_deck *)malloc(sizeof(p_deck));
    newnode->next = NULL;
    newnode->pCard = val;
    if (*head == NULL)
    {
        *head = newnode;
    }
    else
    {
        p_deck *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newnode;
    }
}

// distributing cards to the players
void distribute_cards(p_deck **head1, p_deck **head2, int *p2_piletop, card **p2_pile)
{
    card x;
    for (int i = 0; i < 7; i++)
    {
        x = pop(p2_piletop, p2_pile);
        insert_end(head1, x);
        x = pop(p2_piletop, p2_pile);
        insert_end(head2, x);
    }
}

// shows the player's cards
void displayPlayerDeck(p_deck *head)
{
    int i = 1;
    if (head == NULL)
    {
        printf("cards are over\n");
    }
    while (head != NULL)
    {
        printf("%d) card number :%d card colour:%c \n", i, head->pCard.num, head->pCard.colour);
        head = head->next;
        i++;
    }
}

// shows the cards present in a particular stack
void displayColorStack(int top, card *stk)
{
    int i = 1;
    // printf("cards of color %c\n", stk[top].colour);
    while (top != -1)
    {
        printf("%d) Card color : %c    Card Number : %d\n", i, stk[top--].colour, stk[top].num);
        i++;
        // printf("%d\n", stk[top--].num);
    }
}

// pushing the card onto the pile
void push(card add, card **p2_p_Pile, int *p2_p_piletop)
{
    *p2_p_Pile = (card *)malloc(sizeof(card) * 88);
    (*p2_p_Pile)[++(*p2_p_piletop)] = add;
}

// this checks the match for the color and number present at top of the playing pile in the player's deck
int card_check(p_deck **head, card val, int pos)
{
    pos = 1;
    p_deck *current = *head;
    while (current != NULL)
    {
        if (current->pCard.num == val.num || current->pCard.colour == val.colour)
        {
            return pos;
        }
        else
        {
            current = current->next;
            pos++;
        }
    }
    return -1;
}

int countCards(p_deck *head)
{
    int count = 0;
    while (head != NULL)
    {
        count++;
        head = head->next;
    }
    return count;
}
// deletes a card at a position from the player's deck during a play

card delete_at_pos(p_deck **head, int pos)
{
    p_deck *temp = *head;
    p_deck *prev = temp;
    card return_val;
    if (pos == 1)
    {
        *head = (*head)->next;
        return_val = temp->pCard;
        free(temp);
        return return_val;
    }
    else
    {
        for (int i = 0; i < pos - 1; i++)
        {
            prev = temp;
            temp = temp->next;
        }
        return_val = temp->pCard;
        prev->next = temp->next;
        temp->next = NULL;
        free(temp);

        return return_val;
    }
}

// checks if there is a +2 card in the player's deck
int card_plus_2_check(p_deck **head, card val, int pos)
{
    pos = 1;
    p_deck *current = *head;
    while (current != NULL)
    {
        if (current->pCard.num == 10 && current->pCard.colour == val.colour)
        {
            return pos;
        }
        else
        {
            current = current->next;
            pos++;
        }
    }
    return -1;
}

// main function that plays the game
void play_uno(p_deck **head1, p_deck **head2, int *p2_piletop, card **p2_pile, card **p2_p_Pile, int *p2_p_piletop)
{
    card removed;
    card val;
    int pos = -1;
    card top;
    int i = 1; // to represent the round number being played
    card opening_card = pop(p2_piletop, p2_pile);
    printf("opening card number:%d colour:%c\n", opening_card.num, opening_card.colour);
    push(opening_card, p2_p_Pile, p2_p_piletop); // adding opening card to the player pile

    while ((*head1) != NULL || (*head2) != NULL)
    {
        // displaying cards that each player has after a play :
        printf(" ROUND %d", i);

        printf("\n Computer has : \n");
        displayPlayerDeck(*head1);

        int match1 = card_check(head1, (*p2_p_Pile)[*p2_p_piletop], pos);        // returning the position of the matched card for
        int match2 = card_plus_2_check(head1, (*p2_p_Pile)[*p2_p_piletop], pos); // returning the position of the +2 card
        if (match1 != -1 && match2 == -1)                                        // there is no +2 card
        {
            removed = delete_at_pos(head1, match1); // if found remove from sll and push onto player deck
            top = removed;
            printf("new top card number player1:%d color:%c\n", removed.num, removed.colour);
            push(removed, p2_p_Pile, p2_p_piletop);
            if (*head1 == NULL)
            { // now if head1 ==NULL ie empty return
                printf("Player 1 is the winner\n");
                return; // Exit the loop and the function
            }
        }
        else if (match1 == -1 && match2 != -1) //+2 card found and there is no other match
        {
            removed = delete_at_pos(head1, match2); // if found remove from sll and push onto player deck
            top = removed;
            push(removed, p2_p_Pile, p2_p_piletop);
            printf("the +2 card has been drawn\n");
            printf("new top card number player1:%d color:%c\n", removed.num, removed.colour);
            val = pop(p2_piletop, p2_pile);
            insert_end(head2, val);
            val = pop(p2_piletop, p2_pile);
            insert_end(head2, val);
            if (*head1 == NULL)
            { // now if head1 ==NULL ie empty return
                printf("Player 1 is the winner\n");
                return; // Exit the loop and the function
            }
        }
        else if (match1 != -1 && match2 != -1)
        {
            removed = delete_at_pos(head1, match2); // if found remove from sll and push onto player deck
            top = removed;
            push(removed, p2_p_Pile, p2_p_piletop);
            printf("the +2 card has been drawn\n");
            printf("new top card number player1:%d color:%c\n", removed.num, removed.colour);
            val = pop(p2_piletop, p2_pile);
            insert_end(head2, val);
            val = pop(p2_piletop, p2_pile);
            insert_end(head2, val);
            if (*head1 == NULL)
            { // now if head1 ==NULL ie empty return
                printf("Player 1 is the winner\n");
                return; // Exit the loop and the function
            }
        }
        else // no matching card found
        {
            printf("new top card number:%d color:%c\n", top.num, top.colour);
            val = pop(p2_piletop, p2_pile);
            if (p2_piletop == NULL)
            {
                printf("game is draw\n");
            }
            insert_end(head1, val); // if card not found draw one from the shuffled pile
        }
        /////////////////////////////// COMPUTER PLAYS TILL ABOVE STEPS AND NOW MOVES TO PLAYER FROM BELOW ////////////////////////////////////

        printf("\n User has : \n");
        displayPlayerDeck(*head2);

        // removing the card from the deck that player wants to play
        int index;

        // checkig if the card index is valid
        while (1)
        {
            printf("Card index you want to play - if no matching card, enter -1 : ");
            scanf("%d", &index);

            if (index <= countCards(*head2))
            {
                break;
            }
            else
            {
                continue;
            }
        }

        if (index != -1)
        {
            removed = delete_at_pos(head2, index); // if found remove from sll and push onto player deck
            top = removed;
            if (removed.num != 10)
            {

                printf("new top card number that was played by user:%d color:%c\n", removed.num, removed.colour);
                push(removed, p2_p_Pile, p2_p_piletop);
                if (*head2 == NULL)
                {
                    printf("User is the winner\n");
                    return; // Exit the loop and the function
                }
            }
            else
            {
                printf("new top card number player1:%d color:%c\n", removed.num, removed.colour);
                push(removed, p2_p_Pile, p2_p_piletop);
                printf("the +2 card has been drawn\n");
                val = pop(p2_piletop, p2_pile);
                insert_end(head1, val);
                val = pop(p2_piletop, p2_pile);
                insert_end(head1, val);
                if (*head2 == NULL)
                { // now if head1 ==NULL ie empty return
                    printf("Player 1 is the winner\n");
                    return; // Exit the loop and the function
                }
                printf("new top card number player1:%d color:%c\n", removed.num, removed.colour);
            }
        }
        else
        {
            printf("new top card number player:%d color:%c\n", top.num, top.colour);
            val = pop(p2_piletop, p2_pile); // if card not found draw one from the shuffled pile
            if (p2_piletop == NULL)
            {
                printf("game is draw\n");
            }
            insert_end(head2, val);
            printf("\nUser drew card : Number %d Color %c\n", val.num, val.colour);
        }
        i++;
    }

    if ((*head1) == NULL)
    {
        printf("\nComputer is the winner!!\n");
    }
    else
    {
        printf("\nUser is the winner!!!\n");
    }
}

int main()
{
    srand(time(NULL)); // setting seed to current time - otherwise same seed always
    int RedTop = -1;
    int BlueTop = -1;
    int GreenTop = -1;
    int YellowTop = -1;
    int SPile_Top = -1;
    int PPile_top = -1;

    card *RedStk = NULL;
    card *BlueStk = NULL;
    card *GreenStk = NULL;
    card *YellowStk = NULL;
    card *Pile = NULL;
    card *P_Pile = NULL;
    populateColorStk(1, &RedTop, &RedStk);
    populateColorStk(2, &BlueTop, &BlueStk);
    populateColorStk(3, &GreenTop, &GreenStk);
    populateColorStk(4, &YellowTop, &YellowStk);

    // sdisplayColorStack(RedTop, RedStk);
    create_semi_Shuffled_Pile(&RedStk, &RedTop, &GreenStk, &GreenTop, &BlueStk, &BlueTop, &YellowStk, &YellowTop, &Pile, &SPile_Top);
    shuffle(Pile, 88);
    shuffle(Pile, 88);
    p_deck *head1 = NULL;
    p_deck *head2 = NULL;
    distribute_cards(&head1, &head2, &SPile_Top, &Pile);

    printf("GAME HAS BEGUNNNNNNN!!!!\n");
    play_uno(&head1, &head2, &SPile_Top, &Pile, &P_Pile, &PPile_top);
    printf("\nafter playing the game : \n");
    printf("cards of player one\n");
    displayPlayerDeck(head1);
    printf("cards of player 2\n");
    displayPlayerDeck(head2);
}