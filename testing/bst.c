/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 20:26:28 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/09 22:58:59 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	int			data;
	struct Node	*left;
	struct Node	*right;
}				Node;

void	printTree(Node *root, int space)
{
	if (!root)
		return ;
	space += 10;
	printTree(root->left, space);
	printf("\n");
	for (int i = 0; i < space; i++)
		printf(" ");
	printf("%d\n", root->data);
	printTree(root->right, space);
}

void	init_bst(Node *first, int x)
{
	Node	*new_node;
	Node	*node;

	if (first == NULL)
	{
		node = malloc(sizeof(Node));
		node->data = x;
		node->left = node->right = NULL;
		return ;
	}
	while (first)
	{
		if (x <= first->data)
		{
			if (first->left == NULL)
			{
				new_node = malloc(sizeof(Node));
				new_node->data = x;
				new_node->left = new_node->right = NULL;
				first->left = new_node;
				return ;
			}
			first = first->left;
		}
		if (x >= first->data)
		{
			if (first->right == NULL)
			{
				new_node = malloc(sizeof(Node));
				new_node->data = x;
				new_node->left = new_node->right = NULL;
				first->right = new_node;
				return ;
			}
			first = first->right;
		}
	}
}
int	main(void)
{
	Node *first;
	// Node *child1;
	// Node *child2;
	// Node *child3;
	// Node *child4;
	// Node *child5;
	// Node *child6;

	first = malloc(sizeof(Node));
	// child1 = malloc(sizeof(Node));
	// child2 = malloc(sizeof(Node));
	// child3 = malloc(sizeof(Node));
	// child4 = malloc(sizeof(Node));
	// child5 = malloc(sizeof(Node));
	// child6 = malloc(sizeof(Node));

	first->data = 500;
	first->left = NULL;
	first->right = NULL;
	// child1->data = 70;
	// child2->data = 20;
	// child3->data = 90;
	// child4->data = 100;
	// child5->data = 15;
	// child6->data = 37;

	init_bst(first, 800);
	init_bst(first, 900);
	init_bst(first, 90);
	init_bst(first, 300);
	init_bst(first, 20);
	init_bst(first, -20);
	init_bst(first, 0);
	init_bst(first, 10);
	init_bst(first, 50);
	init_bst(first, 4);
	init_bst(first, 10);
	init_bst(first, 400);
	init_bst(first, 430);
	init_bst(first, 830);
	printTree(first, 10);
}