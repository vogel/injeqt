/*
 * %injeqt copyright begin%
 * Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %injeqt copyright end%
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "sorted-unique-vector.h"

#include <QtTest/QtTest>

using namespace injeqt::internal;

class sorted_unique_vector_test : public QObject
{
	Q_OBJECT

	static int extract_key(const int &x)
	{
		return x;
	}

	static int extract_key_pair(const std::pair<int, std::string> &x)
	{
		return x.first;
	}

	using suv_int = sorted_unique_vector<int, int, extract_key>;
	using suv_pair = sorted_unique_vector<int, std::pair<int, std::string>, extract_key_pair>;

private slots:
	void should_be_empty_after_default_construction();
	void should_be_empty_after_clear();
	void should_be_valid_after_conversion_from_unique_vector();
	void should_be_valid_after_conversion_from_non_unique_vector();
	void should_be_valid_after_conversion_from_unique_sorted_vector();
	void should_be_valid_after_conversion_from_non_unique_sorted_vector();
	void should_be_valid_after_adding_less_than_smallest_element();
	void should_be_valid_after_adding_smallest_element();
	void should_be_valid_after_adding_medium_element();
	void should_be_valid_after_adding_largest_element();
	void should_be_valid_after_adding_greater_than_largest_element();
	void should_be_valid_after_merging_itself();
	void should_be_valid_after_merging_lesser_elements();
	void should_be_valid_after_merging_lesser_or_equal_elements();
	void should_be_valid_after_merging_misc_elements();
	void should_be_valid_after_merging_misc_unique_elements();
	void should_be_valid_after_merging_greater_or_equal_elements();
	void should_be_valid_after_merging_greater_elements();
	void should_match_return_nothing_for_two_empty_vectors();
	void should_match_return_only_unresolved_for_first_empty_vector();
	void should_match_return_only_unresolved_for_second_empty_vector();
	void should_match_return_only_unresolved_for_non_matching_vectors();
	void should_match_return_only_resolved_for_matching_vectors();
	void should_match_return_valid_data_for_partially_matching_vectors();
	void should_return_false_for_contains_when_empty();
	void should_return_false_for_contains_when_does_not_contain();
	void should_return_true_for_contains_when_contains();
	void should_return_false_for_contains_when_contains_key_with_different_value();
	void should_return_false_for_contains_key_when_empty();
	void should_return_false_for_contains_when_does_not_contain_key();
	void should_return_true_for_contains_when_contains_key();

};

void sorted_unique_vector_test::should_be_empty_after_default_construction()
{
	auto data = suv_int{};

	QVERIFY(data.empty());
	QCOMPARE(data.size(), 0ul);
	QCOMPARE(data.content(), (std::vector<int>{}));
}

void sorted_unique_vector_test::should_be_empty_after_clear()
{
	auto data = suv_int{1, 4, 5, 2};

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);

	data.clear();

	QVERIFY(data.empty());
	QCOMPARE(data.size(), 0ul);
}

void sorted_unique_vector_test::should_be_valid_after_conversion_from_unique_vector()
{
	auto data = suv_int{1, 4, 5, 2};

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_conversion_from_non_unique_vector()
{
	auto data = suv_int{1, 4, 5, 2, 1, 4, 5, 2};

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_conversion_from_unique_sorted_vector()
{
	auto data = suv_int{1, 2, 4, 5};

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_conversion_from_non_unique_sorted_vector()
{
	auto data = suv_int{1, 1, 2, 2, 4, 4, 5, 5};

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_adding_less_than_smallest_element()
{
	auto data = suv_int{1, 2, 4, 5};
	data.add(0);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 5ul);
	QCOMPARE(data.content(), (std::vector<int>{0, 1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_adding_smallest_element()
{
	auto data = suv_int{1, 2, 4, 5};
	data.add(1);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_adding_medium_element()
{
	auto data = suv_int{1, 2, 4, 5};
	data.add(3);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 5ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 3, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_adding_largest_element()
{
	auto data = suv_int{1, 2, 4, 5};
	data.add(5);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_adding_greater_than_largest_element()
{
	auto data = suv_int{1, 2, 4, 5};
	data.add(6);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 5ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5, 6}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_itself()
{
	auto data = suv_int{1, 2, 4, 5};
	data.merge(data);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 4ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_lesser_elements()
{
	auto data = suv_int{1, 2, 4, 5};
	auto data_to_add = suv_int{0, -1, -2};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 7ul);
	QCOMPARE(data.content(), (std::vector<int>{-2, -1, 0, 1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_lesser_or_equal_elements()
{
	auto data = suv_int{1, 2, 4, 5};
	auto data_to_add = suv_int{1, 0, -1, -2};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 7ul);
	QCOMPARE(data.content(), (std::vector<int>{-2, -1, 0, 1, 2, 4, 5}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_misc_elements()
{
	auto data = suv_int{0, 5, 10, 1, 3, 2};
	auto data_to_add = suv_int{-1, 17, 2, 3, 4, 5};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 9ul);
	QCOMPARE(data.content(), (std::vector<int>{-1, 0, 1, 2, 3, 4, 5, 10, 17}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_misc_unique_elements()
{
	auto data = suv_int{0, 5, 10, 1, 3, 2};
	auto data_to_add = suv_int{-1, 17, 4};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 9ul);
	QCOMPARE(data.content(), (std::vector<int>{-1, 0, 1, 2, 3, 4, 5, 10, 17}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_greater_or_equal_elements()
{
	auto data = suv_int{1, 2, 4, 5};
	auto data_to_add = suv_int{7, 8, 6};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 7ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5, 6, 7, 8}));
}

void sorted_unique_vector_test::should_be_valid_after_merging_greater_elements()
{
	auto data = suv_int{1, 2, 4, 5};
	auto data_to_add = suv_int{7, 5, 6};
	data.merge(data_to_add);

	QVERIFY(!data.empty());
	QCOMPARE(data.size(), 6ul);
	QCOMPARE(data.content(), (std::vector<int>{1, 2, 4, 5, 6, 7}));
}

void sorted_unique_vector_test::should_match_return_nothing_for_two_empty_vectors()
{
	auto result = match(suv_int{}, suv_int{});
	QVERIFY(result.matched.empty());
	QVERIFY(result.unmatched_1.empty());
	QVERIFY(result.unmatched_2.empty());
}

void sorted_unique_vector_test::should_match_return_only_unresolved_for_first_empty_vector()
{
	auto result = match(suv_int{}, suv_int{});
	QVERIFY(result.matched.empty());
	QVERIFY(result.unmatched_1.empty());
	QVERIFY(result.unmatched_2.empty());
}

void sorted_unique_vector_test::should_match_return_only_unresolved_for_second_empty_vector()
{
	auto result = match(suv_int{}, suv_int{1, 2, 3, 4, 5});
	QVERIFY(result.matched.empty());
	QVERIFY(result.unmatched_1.empty());
	QCOMPARE(result.unmatched_2.content(), (std::vector<int>{1, 2, 3, 4, 5}));
}

void sorted_unique_vector_test::should_match_return_only_unresolved_for_non_matching_vectors()
{
	auto result = match(suv_int{1, 2, 3, 4, 5}, suv_int{});
	QVERIFY(result.matched.empty());
	QCOMPARE(result.unmatched_1.content(), (std::vector<int>{1, 2, 3, 4, 5}));
	QVERIFY(result.unmatched_2.empty());
}

void sorted_unique_vector_test::should_match_return_only_resolved_for_matching_vectors()
{
	auto result = match(suv_int{1, 2, 3, 4, 5}, suv_int{1, 2, 3, 4, 5});
	QCOMPARE(result.matched, (std::vector<std::pair<int, int>>{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}}));
	QVERIFY(result.unmatched_1.empty());
	QVERIFY(result.unmatched_2.empty());
}

void sorted_unique_vector_test::should_match_return_valid_data_for_partially_matching_vectors()
{
	auto result = match(suv_int{1, 2, 3}, suv_int{2, 3, 4, 5});
	QCOMPARE(result.matched, (std::vector<std::pair<int, int>>{{2, 2}, {3, 3}}));
	QCOMPARE(result.unmatched_1.content(), (std::vector<int>{1}));
	QCOMPARE(result.unmatched_2.content(), (std::vector<int>{4, 5}));
}

void sorted_unique_vector_test::should_return_false_for_contains_when_empty()
{
	auto data = suv_pair{};
	QVERIFY(!data.contains(std::make_pair(0, std::string{"0"})));
}

void sorted_unique_vector_test::should_return_false_for_contains_when_does_not_contain()
{
	auto data = suv_pair{std::make_pair(0, std::string{"0"})};
	QVERIFY(!data.contains(std::make_pair(1, std::string{"1"})));
}

void sorted_unique_vector_test::should_return_true_for_contains_when_contains()
{
	auto data = suv_pair{std::make_pair(0, std::string{"0"})};
	QVERIFY(data.contains(std::make_pair(0, std::string{"0"})));
}

void sorted_unique_vector_test::should_return_false_for_contains_when_contains_key_with_different_value()
{
	auto data = suv_pair{std::make_pair(0, std::string{"0"})};
	QVERIFY(!data.contains(std::make_pair(0, std::string{"1"})));
}

void sorted_unique_vector_test::should_return_false_for_contains_key_when_empty()
{
	auto data = suv_pair{};
	QVERIFY(!data.contains_key(0));
}

void sorted_unique_vector_test::should_return_false_for_contains_when_does_not_contain_key()
{
	auto data = suv_pair{std::make_pair(0, std::string{"0"})};
	QVERIFY(!data.contains_key(1));
}

void sorted_unique_vector_test::should_return_true_for_contains_when_contains_key()
{
	auto data = suv_pair{std::make_pair(0, std::string{"0"})};
	QVERIFY(data.contains_key(0));
}


QTEST_APPLESS_MAIN(sorted_unique_vector_test)
#include "sorted-unique-vector-test.moc"
