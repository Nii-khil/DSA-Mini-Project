#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char fname[100];
    struct Node *left;
    struct Node *right;
} SLOT;

typedef struct llnode
{
    char fname[100];
    struct llnode *next;
} LIST;

LIST *createList(char fname[])
{
    LIST *newNode = (LIST *)malloc(sizeof(LIST));
    strcpy(newNode->fname, fname);
    newNode->next = NULL;
    return newNode;
}

SLOT *createFile(char fname[])
{
    SLOT *newNode = (SLOT *)malloc(sizeof(SLOT));
    strcpy(newNode->fname, fname);
    newNode->left = newNode->right = NULL;
    return newNode;
}

SLOT *insertFile(SLOT *root, char fname[])
{
    if (root == NULL)
    {
        printf("File %s added\n", fname);
        return createFile(fname);
    }
    if (strcmp(fname, root->fname) < 0)
    {
        root->left = insertFile(root->left, fname);
    }
    else if (strcmp(fname, root->fname) > 0)
    {
        root->right = insertFile(root->right, fname);
    }
    else
    {
        printf("Another file with this name already exists.\n");
    }
    return root;
}

SLOT *leftmostnode(SLOT *node)
{
    SLOT *temp = node;
    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}

SLOT *deleteFile(SLOT *root, char fname[])
{
    if (root == NULL)
    {
        return root;
    }
    if (strcmp(fname, root->fname) < 0)
    {
        root->left = deleteFile(root->left, fname);
    }
    else if (strcmp(fname, root->fname) > 0)
    {
        root->right = deleteFile(root->right, fname);
    }
    else
    {
        if (root->left == NULL)
        {
            SLOT *temp = root->right;
            free(root);
            printf("File %s deleted\n", fname);
            return temp;
        }
        else if (root->right == NULL)
        {
            SLOT *temp = root->left;
            free(root);
            printf("File %s deleted\n", fname);
            return temp;
        }
        SLOT *temp = leftmostnode(root->right);
        strcpy(root->fname, temp->fname);
        root->right = deleteFile(root->right, temp->fname);
    }
    return root;
}

void inOrderTraversal(SLOT *root)
{
    if (root != NULL)
    {
        inOrderTraversal(root->left);
        printf("%s\n", root->fname);
        inOrderTraversal(root->right);
    }
}

SLOT *searchFile(SLOT *root, char fname[])
{
    while (root != NULL)
    {
        int findFile = strcmp(fname, root->fname);
        if (findFile == 0)
        {
            printf("File found: %s\n", fname);
            return root;
        }
        else if (findFile < 0)
        {
            root = root->left;
        }
        else
        {
            root = root->right;
        }
    }
    printf("File not found: %s\n", fname);
    return NULL;
}

LIST *insertToList(LIST *head, char fname[])
{
    LIST *temp = createList(fname);
    if (head == NULL)
    {
        head = temp;
    }
    else
    {
        LIST *cur = head;
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        cur->next = temp;
    }
    return head;
}

void displayFilesWithExtension(SLOT *root, char extension[]) {
    SLOT *stack[100]; 
    int top = -1;
    int found = 0;

    while (root != NULL || top != -1) {
        while (root != NULL) {
            stack[++top] = root;
            root = root->left;
        }

        root = stack[top--];
       
        char *ext = strrchr(root->fname, '.');
        
        if (ext != NULL && strcmp(ext + 1, extension) == 0) {
            printf("%s\n", root->fname);
            found = 1;
        }

        root = root->right;
    }

    if (found == 0) {
        printf("No files found with the extension '%s'\n", extension);
    }
}

LIST *restoreDeletedFile(LIST *head, char fname[])
{
    if (head == NULL)
    {
        printf("File not found to restore: %s\n", fname);
        return NULL;
    }
    else
    {
        LIST *cur = head;
        while (cur != NULL)
        {
            if (strcmp(fname, cur->fname) == 0)
            {
                return cur;
            }
            else
            {
                cur = cur->next;
            }
        }
        printf("File not found to restore: %s\n", fname);
        return NULL;
    }
}

int main()
{
    SLOT *root = NULL;
    LIST *head = NULL;
    LIST *restore;
    char name[100];
    int ch;
    while (1)
    {
        printf("\nWhat would you like to do:\n");
        printf("0. Exit\n1. Create a file\n2. Delete a file\n3. Display directory\n4. Search for a file\n");
        printf("5. Restore a deleted file\n6. Display files with a specific extension\n");
        printf("Enter your choice:\n");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            printf("Enter the name of the file that you would like to create:\n");
            scanf("%s", name);
            root = insertFile(root, name);
            break;
        case 2:
            printf("Enter the name of the file that you would like to delete:\n");
            scanf("%s", name);
            if (searchFile(root, name) != NULL)
            {
                head = insertToList(head, name);
            }
            root = deleteFile(root, name);
            break;
        case 3:
            printf("Displaying all the files in the current directory:\n");
            inOrderTraversal(root);
            break;
        case 4:
            printf("Enter the name of the file you wish to search for:\n");
            scanf("%s", name);
            searchFile(root, name);
            break;
        case 5:
            printf("Enter the name of the file you would like to restore:\n");
            scanf("%s", name);
            restore = restoreDeletedFile(head, name);
            if (restore != NULL)
            {
                root = insertFile(root, restore->fname);
            }
            break;
        case 6:
            printf("Enter the extension to display files with that extension:\n");
            scanf("%s", name);
            displayFilesWithExtension(root, name);
            break;
        case 0:
            exit(0);
        default:
            break;
        }
    }
    printf("Printing all the files in the directory:\n");
    return 0;
}