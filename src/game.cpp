#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <cmath>
#include <format>
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
#define clr()   \
	move(0, 0); \
	clrtobot()
class Upgrade
{
public:
	std::string name;
	int lvl;
	int mult;
	float income;
	float cost;
	Upgrade(std::string upgrade_name, int level, int factor)
	{
		name = upgrade_name;
		lvl = level;
		mult = factor;
		income = 0;
		cost = 0;
	}
};
class UpgradeList
{
public:
	std::vector<Upgrade> upgrades;
	UpgradeList(std::vector<Upgrade> list = std::vector<Upgrade>{}) { upgrades = list; }
	UpgradeList new_upgrade(std::string name)
	{
		upgrades.push_back(Upgrade(name, 0, upgrades.size() + 1));
		return *this;
	}
	std::vector<Upgrade> &get() { return upgrades; }
	Upgrade &at(int n) { return get().at(n); }
	int size() { return get().size(); }
	Upgrade &cheapest()
	{
		for (Upgrade &u : upgrades)
		{
			if (u.cost == 0)
			{
				return u;
			}
		}
		Upgrade *cheapest_paid = nullptr;
		for (Upgrade &u : upgrades)
		{
			if (u.cost <= 0)
			{
				continue;
			}
			if (!cheapest_paid || u.cost < cheapest_paid->cost)
			{
				cheapest_paid = &u;
			}
		}
		return *cheapest_paid;
	}
};
int get_n(Upgrade u, int lvl = 0)
{
	return (lvl ? lvl : u.lvl) * std::pow(10, u.mult);
};
struct
{
	struct
	{
		std::string title;
		int tick;
	} config;
	float money;
	float income;
	UpgradeList upgrades;
} State;
int main()
{
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, true);
	State.config.title = "C++ Incremental";
	State.config.tick = 1000;
	State.money = 100;
	State.upgrades = UpgradeList()
						 .new_upgrade("I")
						 .new_upgrade("II")
						 .new_upgrade("III")
						 .new_upgrade("IV")
						 .new_upgrade("V");
	char ch;
	bool running = true;
	bool menu = true;
	while (running && menu)
	{
		switch (getch())
		{
		case ' ':
			menu = false;
			break;
		case 'q':
			running = false;
			break;
		}
		struct
		{
			int v, h;
		} offset;
		offset.v = 0;
		offset.h = 0;
		mvprintw(offset.v++, 0, State.config.title.c_str());
		mvprintw(offset.v++, 0, "Controls:");
		mvprintw(offset.v++, 4, "<%c>: Quit", 'q');
		mvprintw(offset.v++, 4, "<%s>: Buy Upgrade", "1-9,0");
		mvprintw(offset.v++, 4, "<%s>: Buy Cheapest", "space");
		mvprintw(offset.v++, 0, "Press <%s> to play!", "space");
		refresh();
	}
	while (running)
	{
		ch = getch();
		if (ch != ERR)
		{
			if (ch == 'q')
			{
				running = false;
			}
		}
		State.income = 0;
		for (int i = 0; i < State.upgrades.size(); i++)
		{
			State.income += State.upgrades.at(i).income;
		}
		State.money += State.income / State.config.tick / 20;
		struct
		{
			int h, v;
		} offset;
		clr();
		offset.v = 0;
		offset.h = 8;
		mvprintw(offset.v++, 1, "$%.2f", State.money);
		mvprintw(offset.v++, 0, "+$%.2f/s", State.income);
		offset.v++;
		Upgrade *prev = nullptr;
		for (int i = 0; i < State.upgrades.size(); i++)
		{
			Upgrade &u = State.upgrades.at(i);
			u.income = get_n(u);
			u.cost = u.lvl ? u.income * std::sqrt(2) : (prev ? get_n(*prev, 10) : 0);
			if (u.lvl > 0 || (prev && prev->lvl > 0) || !prev)
			{
				int key = i < 9
							  ? i + 1
							  : 0;
				if (((int)ch - 48 == key || ch == ' ') && State.money >= u.cost)
				{
					switch (ch)
					{
					case ' ':
						State.money -= State.upgrades.cheapest().cost;
						State.upgrades.cheapest().lvl++;
						break;
					default:
						State.money -= u.cost;
						u.lvl++;
						break;
					}
				}
				mvprintw(offset.v++, !offset.h, "'%s'", u.name.c_str());
				if (u.lvl)
				{
					mvprintw(offset.v - 1, offset.h, "lvl-%i", u.lvl);
				}
				mvprintw(offset.v, !offset.h, "<%i>", key);
				if (!u.lvl)
				{
					offset.v--;
				}
				mvprintw(offset.v++, offset.h, "Income: $%.2f/s", u.income);
				mvprintw(offset.v++, offset.h, "Cost: %s", (u.cost ? std::format("${:.2f}", u.cost) : "free").c_str());
			}
			prev = &u;
		}
		refresh();
		sleep(1 / (float)60);
	}
	endwin();
	return 0;
}