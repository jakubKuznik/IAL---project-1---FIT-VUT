
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    // Initialize every pointer to NULL
    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    //Freeing space from first list to last 
    list->activeElement = list->firstElement;
    
    // If last element is reached break 
    while (list->activeElement != list->lastElement){
        list->activeElement = list->activeElement->nextElement;
        free(list->activeElement->previousElement);
    }
    // freeing last element simountaniously 
    free(list->activeElement);

    // Init same ass DLL_Init
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

    //temporarily variables
    DLLElementPtr inserting_node;
    DLLElementPtr help_var2;
   
    // Allocation space for new node
    inserting_node = malloc(sizeof(struct DLLElement));
    // If allocation failed
    if(inserting_node == NULL){
        DLL_Error();
        return;
    }

    //Init new node 
    inserting_node->data = data;
    inserting_node->nextElement = NULL;
    inserting_node->previousElement = NULL;


    // If list is not empty
    if(list->firstElement != NULL){
        help_var2 = list->firstElement;
        
        list->firstElement = inserting_node;
        list->firstElement->nextElement = help_var2;
        help_var2->previousElement = list->firstElement;
    //empty list
    }else{
        list->firstElement = inserting_node;
        list->lastElement = inserting_node;
        list->firstElement->nextElement = list->lastElement->nextElement = NULL;
        list->firstElement->previousElement = list->lastElement->previousElement = NULL;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

    DLLElementPtr inserting_node;
    
    // Allocation space for new node
    inserting_node = malloc(sizeof(struct DLLElement));
    // If allocation failed
    if(inserting_node == NULL){
        DLL_Error();
        return;
    }

    //Init new node 
    inserting_node->data = data;
    inserting_node->nextElement = NULL;


    // If list is not empty
    if(list->lastElement != NULL){
       list->lastElement->nextElement = inserting_node;
       inserting_node->previousElement = list->lastElement;
       inserting_node->nextElement = NULL;
       list->lastElement = inserting_node;

    }else{ //Empty list
        list->firstElement = inserting_node;
        list->lastElement = inserting_node;
        list->firstElement->nextElement = list->lastElement->nextElement = NULL;
        list->firstElement->previousElement = list->lastElement->previousElement = NULL;
    }
    return;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    // Return pointer to first element
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
    // Return pointer to last element
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    //Error if want first node of empty list
    if (list->firstElement == NULL){
        DLL_Error();
        return;
    }
    //Returns first node data     
    *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    //Error if want first node of empty list
    if (list->firstElement == NULL){
        DLL_Error();
        return;
    }
    //Returns last node data     
    *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    //If list is empty 
    if(list->firstElement == NULL)
        return;

    // If list has only one element
    if((list->firstElement) == (list->activeElement)){
        list->activeElement = NULL;
    }

    DLLElementPtr help_var;

    /* There is more than one element*/
    if(list->firstElement->nextElement != NULL){
        help_var = list->firstElement->nextElement;
        free(list->firstElement);
        list->firstElement = help_var;
    }
    /* There is just one element */
    else{
        free(list->firstElement);
        list->firstElement = NULL;
        list->lastElement = NULL;
        list->activeElement = NULL;
    }

}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    
    // Doesnt do anything with empty list 
    if(list->lastElement == NULL)
        return;

    // If list has only one element 
    if(list->lastElement == list->activeElement){
        list->activeElement = NULL;
    }

    DLLElementPtr help_var;

    /* There just one element*/
    if(list->firstElement == list->lastElement){
        free(list->lastElement);
        list->firstElement = NULL;
        list->lastElement = NULL;
        list->activeElement = NULL;

    }
    /* There is more elemts*/
    else{
        help_var = list->lastElement->previousElement;
        free(list->lastElement);
        list->lastElement = help_var;
        list->lastElement->nextElement = NULL;
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    // If there is no active element 
    if(list->activeElement == NULL)
        return;
    
    // If active element is last cant delete after it  
    if (list->activeElement == list->lastElement)
        return;

    DLLElementPtr help_var;

    /* If deleting last element. */
    if(list->activeElement->nextElement == list->lastElement){
        free(list->lastElement);
        list->activeElement->nextElement = NULL;
        list->lastElement = list->activeElement;
    }
    else{
        help_var = list->activeElement->nextElement->nextElement;

        free(list->activeElement->nextElement);
        list->activeElement->nextElement = help_var;
        help_var->previousElement = list->activeElement;
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
   
    //If there is no active element
    if(list->activeElement == NULL)
        return;
    //Can delete before first element
    if(list->activeElement == list->firstElement)
        return;

    //Variable initialization  
    DLLElementPtr help_var;
    help_var = list->activeElement->previousElement;
    list->activeElement->previousElement = help_var->previousElement;
    
    //
    if (help_var != list->firstElement) {
        help_var->previousElement->nextElement = list->activeElement;
    } // If deleting first element
    else {
        list->firstElement = list->activeElement;
    }
    free(help_var);
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    // If there is no active element 
    if (list->activeElement == NULL)
        return;
    //var init      
    DLLElementPtr help_var = list->activeElement->nextElement;

    //alocate space
    help_var = malloc(sizeof(struct DLLElement));
    //Not successfull alocation 
    if(help_var == NULL){
        DLL_Error();
        return;
    }
    //variable init 
    help_var->data = data;
    help_var->nextElement = list->activeElement->nextElement;
    help_var->previousElement = list->activeElement;
 
    //If insert to last element 
    if(list->activeElement == list->lastElement){
        list->lastElement = help_var;    
    } 
    else{
        list->activeElement->nextElement->previousElement = help_var;
    }
    list->activeElement->nextElement = help_var;    

}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    // There is no active elem.
    if(list->activeElement == NULL)
        return;
    
    //alocation
    DLLElementPtr help = malloc(sizeof(struct DLLElement));
    //Not succes alocation 
    if (help == NULL){
        DLL_Error();
        return;
    }
    // Var init 
    help->data = data;
    help->previousElement = list->activeElement->previousElement;
    help->nextElement = list->activeElement;
    

    if (list->activeElement != list->firstElement) {
        list->activeElement->previousElement->nextElement = help;
    } // If list is first elem
    else {
        list->firstElement = help;
    }

    list->activeElement->previousElement = help;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

    // If no active error 
    if(list->activeElement == NULL){
        DLL_Error();
        return;
    }
    *dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {


    // If no active dont do anything 
    if(list->activeElement == NULL){
        return;
    }
    list->activeElement->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    // If no active dont do anything 
    if(list->activeElement == NULL)
        return;

    list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    //If no active dont do anything 
    if(list->activeElement == NULL)
        return;

    list->activeElement = list->activeElement->previousElement;

}
/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    return list->activeElement != NULL;
}

/* Konec c206.c */
