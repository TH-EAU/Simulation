#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define _ENTREE_ ft_place_stock(grid, 8, 100, 1)


void	ft_putchar(char c)
{
	write(1,&c,1);
}

void	ft_putstr(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		ft_putchar(str[i]);
		i++;
	}
}

int		ft_putnbr(int nb)
{
	unsigned int conv;

	if (nb < 0)
	{
		ft_putchar('-');
		nb = -nb;
		conv = -nb;
	}
	conv = nb;
	if (conv >= 10)
	{
		ft_putnbr(conv/10);
		ft_putnbr(conv%10);
	}
	else
		ft_putchar(conv + 48);
}

void	ft_display(int grid[4][12][5])
{
	int i;
	int j;
	int r;
	int a;

	a = 0;
	i = 3;
	while (i != -1)
	{
		j = 0;
		r = 0;
		while (j < 12)
		{
			if (j%3 == 0)
			{
				ft_putchar('\t');
				a++;
				r = 0;
			}
			if (grid[i][j][1] == 0)
				grid[i][j][1] = -32;
			/*ft_putstr("C10");
			ft_putchar(a + 48);
			ft_putchar(r + 65);
			ft_putstr("0");
			ft_putnbr(i);*/
			ft_putchar('[');
			ft_putchar(grid[i][j][1] + 64);
			ft_putchar(',');
			ft_putnbr(grid[i][j][0]);
			ft_putchar(',');
			ft_putnbr(grid[i][j][3]);
			/*ft_putchar(',');
			ft_putchar(grid[i][j][2]);*/
			if (grid[i][j][4] != 0)
			{
				ft_putchar(',');
				ft_putnbr(grid[i][j][4]);
			}
			ft_putchar(']');
			ft_putstr(" ");
			r++;
			j++;
		}
		ft_putchar('\n');
		a = 0;
		i--;
	}
}

void	ft_ini(int grid[4][12][5])
{
	int i;
	int j;
	
	i = 0;
	while (i < 4)
	{
		j = 0;
		
		while (j < 12)
		{
			grid[i][j][0] = 0;
			if (i == 0 || i == 1)
				grid[i][j][1] = j + 1;
			else
				grid[i][j][1] = 0;
			if (i == 0)
				grid[i][j][2] = 80;
			else
				grid[i][j][2] = 83;
			grid[i][j][3] = 0;
			grid[i][j][4] = 0;
			j++;
		}
		i++;
	}
}

int		ft_DLC_pri(int grid[4][12][5], int art, int pos[2])
{
	int i;
	int j;
	int prio;

	i = 0;
	prio = 0;
	pos[0] = 0;
	pos[1] = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 12)
		{
			if (grid[i][j][1] == art && grid[i][j][4] > 0)
			{
			 	if (prio == 0)
				{
					prio = grid[i][j][4];
					pos[0] = i;
					pos[1] = j;
				}
				else if (grid[i][j][4] < prio)
				{
					prio = grid[i][j][4];
					pos[0] = i;
					pos[1] = j;
				}
			}
			j++;
		}
		i++;
	}
	ft_putstr(" > Priorité : DLC N°");
	ft_putnbr(prio);
	ft_putchar('\n');
	if (prio == 0)
	{
		ft_putstr(" > Erreur : Pas de pallette en stock\n");
		ft_putstr(" > Etat : ABORT\n");
		return 0;
	}
	return 1;
}

int		ft_fill_stock(int grid[4][12][5], int art, int qte, int x, int y, int prio)
{
	ft_putstr(" > Action : reception\n");
	if (x <= 0)
	{
		ft_putstr(" > Erreur : Une palette ne peut pas se trouver en picking\n");
		ft_putstr(" > Etat : ABORT\n");
		return 0;
	}
	if (x == 1 && grid[x][y][1] != art)
	{
		ft_putstr(" > Erreur : stock réservé à l'article ");
		ft_putchar(grid[x - 1][y][1] + 64);
		ft_putchar('\n');
		ft_putstr(" > Etat : ABORT\n");
		return 0;	
	}
	if (grid[x][y][3] >= 1)
	{
		ft_putstr(" > Erreur : trop de Palettes en ");
		ft_putnbr(x);
		ft_putchar(grid[x - 1][y][1] + 64);
		ft_putchar('\n');
		ft_putstr(" > Etat : ABORT\n");
		return 0;
	}
	grid[x][y][1] = art;
	grid[x][y][0] = grid[x][y][0] + qte;
	grid[x][y][3]++;
	grid[x][y][4] = prio;

	return 1;
}

void 	ft_void_stock(int grid[4][12][5], int x, int y)
{
	if (x > 1)
	{
		grid[x][y][1] = 0;
	}
		grid[x][y][0] = 0;
		grid[x][y][3] = 0;
		grid[x][y][4] = 0;
}

int		ft_procure_picking(int grid[4][12][5], int art)
{
	ft_putstr(" > Action : Reappro picking\n");
	int pos[2];
	int j;

	j = 0;
	if (ft_DLC_pri(grid, art, pos) == 0)
		return 0;
	if (pos[0] == 0 && pos[1] == 1)
		return 0;
	else
	{
		while (j < 12)
		{
			if (grid[0][j][1] == art)
			{
				grid[0][j][0] = grid[0][j][0] + grid[pos[0]][pos[1]][0];
			}
			j++;
		}
		ft_void_stock(grid, pos[0], pos[1]);
	}
	return 1;
}

int		ft_pick(int grid[4][12][5], int art, int qte)
{
	ft_putstr(" > Action : preparation commande\n");
	int j;

	j = 0;
	while (j < 12)
	{	
		if (grid[0][j][0] == 0 && grid[0][j][1] == art)
		{
			ft_putstr(" > Erreur picking article ");
			ft_putchar(grid[0][art - 1][1] + 64);
			ft_putstr(" négatif\n");
			ft_putstr(" > Etat : DEMANDE REAPPRO\n");
			return 2;
		}
		if (grid[0][j][1] == art)
			grid[0][j][0] = grid[0][j][0] - qte;
		j++;
	}
}

int		*ft_find_place_stock(int grid[4][12][5], int art, int pos[2])
{
	int pick;
	int i;
	int j;
	int step;

	i = 1;
	j = 0;
	step = 0;
	while (j < 12 && j != art - 1)
		j++;
	pick = j;
	if (pick == 12)
		ft_putstr(" > Erreur : Article non trouvé\n");

	while (grid[i][j][3] != 0 && i < 4)
	{
		i++;
		if (i == 4 && step == 0)
		{
			j--;
			i = 2;
			step = 1;
		}
		if (i == 4 && step == 1)
		{
			j += 2;
			i = 2;
			step = 2;
		}
		if (i == 4 && step == 2)
		{
			j = 0;
			i = 2;
			step = 3;
		}
		if (i == 4 && step == 3)
		{
			j++;
			i = 2;
		}
		if (j == 12)
		{
			ft_putstr(" > Plus de place en Stock");
			i = 0;
			j = 12;
			break;
		}
	}
	//ft_putnbr(pick);
	pos[0] = i;
	pos[1] = j;
	return pos;
}

void	ft_place_stock(int grid[4][12][5], int art, int nbcol, int prio)
{
	int pos[2];

	ft_find_place_stock(grid, art, pos);
	if (pos[1] == 12)
		ft_putstr(" > Etat : ABORT\n");
	else
		ft_fill_stock(grid, art, nbcol, pos[0], pos[1], prio);
}

int 	main(int argc, char **argv)
{
	int grid[4][12][5]; // 1 - Article		0 - Nb colis		2 - Stock/Picking 		3 - Nb Palettes		4 - Priorité (DLC)
	int test;

	test = 0;
	ft_ini(grid);

	//--------------------CYCLE--------------------//
		ft_putstr("begin");
		ft_display(grid);
		getchar();

	//------------------RECEPTION------------------//
		//_ENTREE_;
		ft_place_stock(grid, 8, 5, 1);
		ft_place_stock(grid, 8, 5, 2);
		ft_place_stock(grid, 8, 5, 3);
		ft_place_stock(grid, 8, 5, 4);
		ft_place_stock(grid, 8, 5, 5);
		ft_place_stock(grid, 8, 5, 6);
		ft_display(grid);
		getchar();

	//-----------------PREPARATION-----------------//
		while (test < 25)
		{
			if (ft_pick(grid, 8, 1) == 2)
			{
				getchar();
				ft_display(grid);
				if (ft_procure_picking(grid, 8) == 0)
					break;
			}
			test++;
		}
		ft_display(grid);

	return 0;
}
	
	
	
	
	
