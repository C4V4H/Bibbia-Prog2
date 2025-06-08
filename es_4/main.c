/*
 * ==========================================
 * PROGETTO: es_4
 * FILE: main.c
 * AUTORE: Github Copilot (Claude Sonnet 4)
 * DATA: 2025-06-08
 * ==========================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mirror.h"

// Struttura per i test
typedef struct {
    const char *description;
    IntTree (*buildTree)();
    int *expectedPreorder;
    int expectedSize;
} TestCase;

// Funzione per stampare il risultato di un test
void printTestResult(int testNum, const char *description, bool passed) {
    printf("Test %2d: %-50s [%s]\n", 
           testNum, 
           description, 
           passed ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m");
}

// Funzione per eseguire un singolo test
bool runTest(TestCase *test) {
    printf("         Descrizione: %s\n", test->description);
    
    // Costruisci l'albero
    IntTree tree = test->buildTree();
    
    printf("         Albero originale (struttura):\n");
    __printTreeStructure(tree, 0, ' ');
    
    printf("         Preorder originale: [");
    __printTreePreorder(tree);
    printf("]\n");
    
    // Applica mirror
    mirror(tree);
    
    printf("         Albero specchiato (struttura):\n");
    __printTreeStructure(tree, 0, ' ');
    
    printf("         Preorder specchiato: [");
    __printTreePreorder(tree);
    printf("]\n");
    
    // Verifica il risultato
    int nodeCount = __countNodes(tree);
    int *actualPreorder = (int*)malloc(nodeCount * sizeof(int));
    int index = 0;
    __getPreorderArray(tree, actualPreorder, &index);
    
    printf("         Expected preorder: [");
    for (int i = 0; i < test->expectedSize; i++) {
        printf("%d", test->expectedPreorder[i]); // CORREZIONE: era expectedPre order
        if (i < test->expectedSize - 1) printf(" ");
    }
    printf("]\n");
    
    bool passed = true;
    if (nodeCount != test->expectedSize) {
        passed = false;
        printf("         ERROR: Numero di nodi diverso! Expected: %d, Got: %d\n", 
               test->expectedSize, nodeCount);
    } else {
        for (int i = 0; i < test->expectedSize; i++) {
            if (actualPreorder[i] != test->expectedPreorder[i]) {
                passed = false;
                printf("         ERROR: Preorder diverso alla posizione %d! Expected: %d, Got: %d\n", 
                       i, test->expectedPreorder[i], actualPreorder[i]);
                break;
            }
        }
    }
    
    // Cleanup
    __freeTree(tree);
    free(actualPreorder);
    
    return passed;
}

// Funzione lambda per creare albero singolo nodo
IntTree buildSingleNode() {
    return __createNode(42);
}

// Funzione lambda per creare albero vuoto  
IntTree buildEmptyTree() {
    return NULL;
}

int main() {
    printf("=== TEST SUITE per mirror ===\n");
    printf("Author: C4V4H | Date: 2025-06-08\n\n");
    
    // Array di risultati attesi (preorder dopo mirror)
    
    // Test 1: Albero dell'esempio
    // Originale: 2 1 5 4 9  ->  Specchiato: 2 5 9 4 1
    int expected1[] = {2, 5, 9, 4, 1};
    
    // Test 2: Albero più complesso  
    // Originale: 10 5 3 1 7 15 12 20 25  ->  Specchiato: 10 15 20 25 12 5 7 3 1
    int expected2[] = {10, 15, 20, 25, 12, 5, 7, 3, 1};
    
    // Test 3: Albero lineare
    // Originale: 1 2 3 4  ->  Specchiato: 1 2 3 4 (ma con struttura diversa)
    int expected3[] = {1, 2, 3, 4};
    
    // Test 4: Albero completo
    // Originale: 1 2 4 5 3 6 7  ->  Specchiato: 1 3 7 6 2 5 4
    int expected4[] = {1, 3, 7, 6, 2, 5, 4};
    
    // Test 5: Singolo nodo
    int expected5[] = {42};
    
    TestCase tests[] = {
        {"Albero dell'esempio (asimmetrico)", __buildSampleTree1, expected1, 5},
        {"Albero complesso (asimmetrico)", __buildSampleTree2, expected2, 9},
        {"Albero lineare (solo figli destri)", __buildLinearTree, expected3, 4},
        {"Albero completo (simmetrico)", __buildCompleteTree, expected4, 7},
        {"Singolo nodo", buildSingleNode, expected5, 1},
        {"Albero vuoto", buildEmptyTree, NULL, 0}
    };
    
    int totalTests = sizeof(tests) / sizeof(TestCase);
    int passedTests = 0;
    
    // Esecuzione dei test
    for (int i = 0; i < totalTests; i++) {
        printf("========================================\n");
        
        // Test speciale per albero vuoto
        if (i == totalTests - 1) {
            printf("Test %2d: %-50s ", i + 1, tests[i].description);
            mirror(NULL); // Dovrebbe non fare nulla
            printf("[%s]\n", "\033[32mPASS\033[0m");
            passedTests++;
            printf("\n");
            continue;
        }
        
        bool passed = runTest(&tests[i]);
        printTestResult(i + 1, tests[i].description, passed);
        
        if (passed) {
            passedTests++;
        }
        printf("\n");
    }
    
    // Test aggiuntivo: doppio mirror dovrebbe restituire l'originale
    printf("========================================\n");
    printf("Test BONUS: Doppio mirror restituisce originale\n");
    IntTree originalTree = __buildSampleTree1();
    IntTree copyTree = __copyTree(originalTree);
    
    printf("         Albero originale: [");
    __printTreePreorder(originalTree);
    printf("]\n");
    
    // Primo mirror
    mirror(copyTree);
    printf("         Dopo primo mirror: [");
    __printTreePreorder(copyTree);
    printf("]\n");
    
    // Secondo mirror
    mirror(copyTree);
    printf("         Dopo secondo mirror: [");
    __printTreePreorder(copyTree);
    printf("]\n");
    
    bool bonusTest = __compareTreesStructure(originalTree, copyTree);
    printf("Test BONUS: %-50s [%s]\n", 
           "Doppio mirror = originale", 
           bonusTest ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m");
    
    if (bonusTest) passedTests++;
    totalTests++; // Aggiungi il test bonus al conteggio
    
    __freeTree(originalTree);
    __freeTree(copyTree);
    
    // Risultato finale
    printf("\n==========================================\n");
    printf("RISULTATO FINALE: %d test passati su %d\n", passedTests, totalTests);
    printf("Percentuale di successo: %.1f%%\n", (float)passedTests / totalTests * 100);
    printf("==========================================\n");
    
    if (passedTests == totalTests) {
        printf("🎉 TUTTI I TEST SONO PASSATI! 🎉\n");
        return 0;
    } else {
        printf("❌ Alcuni test sono falliti.\n");
        return 1;
    }
}
