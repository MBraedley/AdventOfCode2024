﻿// Day23.cpp : Source file for your target.
//

#include "Day23.h"
#include "Utils.h"

#include <ranges>
#include <algorithm>

int main()
{
	using std::operator""sv;
	auto startTime = std::chrono::system_clock::now();

	auto input = utils::ReadFormattedInput<R"(([a-z]{2})-([a-z]{2}))">("../Day23_input.txt");

	std::vector<std::pair<std::string, std::string>> connections = std::views::all(input)
		| std::views::transform([](const std::vector<std::string>& connection) { return std::make_pair(connection[1], connection[2]); })
		| std::ranges::to<std::vector<std::pair<std::string, std::string>>>();
	
	//std::vector<std::pair<std::string, std::string>> connections
	//{
	//	{"kh","tc"},
	//	{"qp","kh"},
	//	{"de","cg"},
	//	{"ka","co"},
	//	{"yn","aq"},
	//	{"qp","ub"},
	//	{"cg","tb"},
	//	{"vc","aq"},
	//	{"tb","ka"},
	//	{"wh","tc"},
	//	{"yn","cg"},
	//	{"kh","ub"},
	//	{"ta","co"},
	//	{"de","co"},
	//	{"tc","td"},
	//	{"tb","wq"},
	//	{"wh","td"},
	//	{"ta","ka"},
	//	{"td","qp"},
	//	{"aq","cg"},
	//	{"wq","ub"},
	//	{"ub","vc"},
	//	{"de","ta"},
	//	{"wq","aq"},
	//	{"wq","vc"},
	//	{"wh","yn"},
	//	{"ka","de"},
	//	{"kh","ta"},
	//	{"co","tc"},
	//	{"wh","qp"},
	//	{"tb","vc"},
	//	{"td","yn"},
	//};

	std::set<std::set<std::string>> rings;
	std::map<std::string, std::set<std::string>> connectionGroups;

	for (const auto& [lhs, rhs] : connections)
	{
		auto lhsConns = std::views::all(connections)
			| std::views::filter([&](const std::pair<std::string, std::string>& connection)
				{
					return connection.first == lhs || connection.second == lhs;
				})
			| std::views::transform([&](const std::pair<std::string, std::string>& connection)
				{
					return connection.first == lhs ? connection.second : connection.first;
				})
			| std::views::filter([&](const std::string& connection)
				{
					return connection != rhs;
				})
			| std::ranges::to<std::vector<std::string>>();

		auto rhsConns = std::views::all(connections)
			| std::views::filter([&](const std::pair<std::string, std::string>& connection)
				{
					return connection.first == rhs || connection.second == rhs;
				})
			| std::views::transform([&](const std::pair<std::string, std::string>& connection)
				{
					return connection.first == rhs ? connection.second : connection.first;
				})
			| std::views::filter([&](const std::string& connection)
				{
					return connection != lhs;
				})
			| std::ranges::to<std::vector<std::string>>();

		std::ranges::sort(lhsConns);
		std::ranges::sort(rhsConns);

		std::vector<std::string> out;
		std::ranges::set_intersection(lhsConns, rhsConns, std::back_inserter(out));

		//Part 1
		if (lhs.starts_with("t") || rhs.starts_with("t"))
		{
			for (const auto& ohs : out)
			{
				rings.insert(std::set<std::string>{ lhs, rhs, ohs });
			}
		}

		//Part 2
		std::set<std::string> out2 = std::ranges::to<std::set<std::string>>(out);
		out2.insert(lhs);
		out2.insert(rhs);
		connectionGroups.emplace(lhs, out2);
		connectionGroups.emplace(rhs, out2);
	}

	utils::PrintResult(rings.size(), startTime);

	//Part 2

	//auto lanGroups = std::views::all(connectionGroups) | std::views::filter([&](const std::pair<std::string, std::set<std::string>>& group) -> bool
	//	{
	//		for (const std::string& comp : group.second)
	//		{
	//			if (connectionGroups[comp] != group.second)
	//			{
	//				return false;
	//			}
	//		}
	//		return true;
	//	});

	//auto lanGroup = std::ranges::max(std::views::values(lanGroups), {}, &std::set<std::string>::size);
	//utils::PrintResult(lanGroup | std::views::join_with(","sv) | std::ranges::to<std::string>(), startTime);

	for (const auto& [comp, group] : connectionGroups)
	{
		std::cout << comp << ": ";
		utils::PrintResult(group | std::views::join_with(","sv) | std::ranges::to<std::string>(), startTime);
	}

	return 0;
}
