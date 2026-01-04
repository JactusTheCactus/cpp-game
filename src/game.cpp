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
using namespace std;
class Upgrade
{
public:
	std::string name;
	int lvl;
	int mult;
	double income;
	double cost;
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
	UpgradeList(std::vector<Upgrade> list = std::vector<Upgrade>{})
	{
		upgrades = list;
	}
	UpgradeList new_upgrade(std::string name)
	{
		upgrades.push_back(Upgrade(name, 0, upgrades.size() + 1));
		return *this;
	}
	std::vector<Upgrade> &get()
	{
		return upgrades;
	}
	Upgrade &at(int n)
	{
		return get().at(n);
	}
	int size()
	{
		return get().size();
	}
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
	return (lvl ? lvl : u.lvl) * std::pow(10, u.mult - 1);
};
struct
{
	std::string title;
	double money;
	double income;
	UpgradeList upgrades;
} State;
struct
{
	int v, h;
} offset;
template <typename T>
	requires(std::same_as<T, int> || std::same_as<T, double>)
string fmt_number(T number)
{
	bool neg = number < 0;
	string unformatted;
	if constexpr (is_same_v<T, int>)
	{
		unformatted = format("{}", abs(number));
		string formatted = "";
		reverse(unformatted.begin(), unformatted.end());
		int i = 0;
		for (char c : unformatted)
		{
			if (i && !(i % 3))
			{
				formatted += ',';
			}
			formatted += c;
			i++;
		}
		reverse(formatted.begin(), formatted.end());
		if (neg)
		{
			formatted.insert(formatted.begin(), '-');
		}
		return formatted;
	}
	else
	{
		unformatted = format("{:.2f}", number);
		size_t dot = unformatted.find('.');
		return format("{}.{}",
					  fmt_number<int>(stoi(unformatted.substr(0, dot))),
					  unformatted.substr(dot + 1));
	}
}
int main()
{
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, true);
	State.title = "C++ Incremental";
	State.money = 0;
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
		offset.v = 0;
		offset.h = 0;
		mvprintw(offset.v++, 0, "%s", State.title.c_str());
		mvprintw(offset.v++, 0, "Controls:");
		mvprintw(offset.v++, 4, "[%c]: Quit", 'q');
		mvprintw(offset.v++, 4, "[%s]: Buy Upgrade", "1-5");
		mvprintw(offset.v++, 4, "[%s]: Buy Cheapest", "space");
		mvprintw(offset.v++, 0, "Press [%s] to play!", "space");
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
		State.money += State.income / 2e4;
		clr();
		offset.v = 0;
		offset.h = 8;
		mvprintw(offset.v++, 1, "$%s",
				 fmt_number<double>(State.money).c_str());
		mvprintw(offset.v++, 0, "+$%s/s",
				 fmt_number<int>((int)State.income).c_str());
		offset.v++;
		if (ch == ' ')
		{
			mvprintw(0, 0, "[%i]",
					 State.money >= State.upgrades.cheapest().cost);
			if (State.money >= State.upgrades.cheapest().cost)
			{
				State.money -= State.upgrades.cheapest().cost;
				State.upgrades.cheapest().lvl++;
			}
		}
		Upgrade *prev = nullptr;
		for (int i = 0; i < State.upgrades.size(); i++)
		{
			Upgrade &u = State.upgrades.at(i);
			u.income = get_n(u);
			u.cost = u.lvl
						 ? u.income * std::sqrt(2)
						 : (prev ? get_n(*prev, 10) : 0);
			if (u.lvl > 0 || (prev && prev->lvl > 0) || !prev)
			{
				int key = i < 9
							  ? i + 1
							  : 0;
				if ((int)ch - 48 == key && State.money >= u.cost)
				{
					State.money -= u.cost;
					u.lvl++;
				}
				mvprintw(offset.v++, !offset.h, "<%s>",
						 u.name.c_str());
				if (u.lvl)
				{
					mvprintw(offset.v - 1, offset.h, "lvl-%s",
							 fmt_number<int>(u.lvl).c_str());
					clrtoeol();
				}
				mvprintw(offset.v, !offset.h, "[%i]", key);
				if (!u.lvl)
				{
					offset.v--;
				}
				mvprintw(offset.v++, offset.h, "Income:\t+$%s/s",
						 fmt_number<int>((int)u.income).c_str());
				clrtoeol();
				mvprintw(offset.v++, offset.h, "Cost:\t%s",
						 (u.cost
							  ? std::format("-${}", fmt_number<double>(u.cost))
							  : " free")
							 .c_str());
				if (State.money < u.cost)
				{
					double eta = (u.cost - State.money) / State.income;
					char t = 's';
					struct Step
					{
						char unit;
						double factor;
					};
					constexpr Step steps[] = {{'m', 60},
											  {'h', 60},
											  {'d', 24},
											  {'w', 7},
											  {'M', 4},
											  {'y', 12},
											  {'D', 10},
											  {'c', 10}};
					for (const auto &[unit, factor] : steps)
					{
						if (eta >= factor)
						{
							eta /= factor;
							t = unit;
						}
						else
						{
							break;
						}
					}
					mvprintw(offset.v, offset.h, "ETA:\t %s",
							 State.income <= 0
								 ? "--"
								 : format("{}{}",
										  fmt_number<double>(eta),
										  t)
									   .c_str());
				}
			}
			prev = &u;
		}
		move(0, 0);
		refresh();
		sleep(1 / (double)60);
	}
	endwin();
	return 0;
}