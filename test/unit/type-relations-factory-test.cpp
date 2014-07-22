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

#include "implements-extractor.cpp"
#include "type.cpp"
#include "type-relations.cpp"
#include "type-relations-factory.cpp"

#include <QtTest/QtTest>

using namespace injeqt::v1;

class type_1 : public QObject
{
	Q_OBJECT
};

class type_1_sub_1 : public type_1
{
	Q_OBJECT
};

class type_1_sub_1_sub_1 : public type_1_sub_1
{
	Q_OBJECT
};

class type_1_sub_1_sub_2 : public type_1_sub_1
{
	Q_OBJECT
};

class type_1_sub_2 : public type_1
{
	Q_OBJECT
};

class type_2 : public QObject
{
	Q_OBJECT
};

class type_2_sub_1 : public type_2
{
	Q_OBJECT
};

class type_relations_factory_test : public QObject
{
	Q_OBJECT

public:
	type_relations_factory_test();

private slots:
	void should_create_empty_relations_for_no_types();
	void should_create_unique_relations_for_unrelated_types();
	void should_create_unique_relations_for_unrelated_subtypes();
	void should_create_unique_relations_for_type_with_supertypes();
	void should_create_ambiguous_relations_for_the_same_type();
	void should_create_mixed_relations_for_subtypes();
	void should_create_mixed_relations_for_type_and_subtype();
	void should_create_mixed_relations_for_subtype_and_type();

private:
	type type_1_type;
	type type_1_sub_1_type;
	type type_1_sub_1_sub_1_type;
	type type_1_sub_1_sub_2_type;
	type type_1_sub_2_type;
	type type_2_type;
	type type_2_sub_1_type;

	void verify_type(const std::vector<type> &types, const type &t);

};

type_relations_factory_test::type_relations_factory_test() :
	type_1_type{std::addressof(type_1::staticMetaObject)},
	type_1_sub_1_type{std::addressof(type_1_sub_1::staticMetaObject)},
	type_1_sub_1_sub_1_type{std::addressof(type_1_sub_1_sub_1::staticMetaObject)},
	type_1_sub_1_sub_2_type{std::addressof(type_1_sub_1_sub_2::staticMetaObject)},
	type_1_sub_2_type{std::addressof(type_1_sub_2::staticMetaObject)},
	type_2_type{std::addressof(type_2::staticMetaObject)},
	type_2_sub_1_type{std::addressof(type_2_sub_1::staticMetaObject)}
{
}

void type_relations_factory_test::verify_type(const std::vector<type> &types, const type &t)
{
	auto it = std::find(std::begin(types), std::end(types), t);
	QVERIFY(it != std::end(types));
}

void type_relations_factory_test::should_create_empty_relations_for_no_types()
{
	auto relations = type_relations_factory{}.create_type_relations({});

	QVERIFY(relations.unique().empty());
	QVERIFY(relations.ambiguous().empty());
}

void type_relations_factory_test::should_create_unique_relations_for_unrelated_types()
{
	auto relations = type_relations_factory{}.create_type_relations({type_1_type, type_2_type});

	QCOMPARE(relations.unique().size(), 2ul);
	verify_type(relations.unique(), type_1_type);
	verify_type(relations.unique(), type_2_type);
	QVERIFY(relations.ambiguous().empty());
}

void type_relations_factory_test::should_create_unique_relations_for_unrelated_subtypes()
{
	auto relations = type_relations_factory{}.create_type_relations({type_1_sub_1_sub_1_type, type_2_sub_1_type});

	QCOMPARE(relations.unique().size(), 5ul);
	verify_type(relations.unique(), type_1_type);
	verify_type(relations.unique(), type_1_sub_1_type);
	verify_type(relations.unique(), type_1_sub_1_sub_1_type);
	verify_type(relations.unique(), type_2_type);
	verify_type(relations.unique(), type_2_sub_1_type);
	QVERIFY(relations.ambiguous().empty());
}

void type_relations_factory_test::should_create_unique_relations_for_type_with_supertypes()
{
	auto relations = type_relations_factory{}.create_type_relations({type_1_sub_1_sub_1_type});

	QCOMPARE(relations.unique().size(), 3ul);
	verify_type(relations.unique(), type_1_type);
	verify_type(relations.unique(), type_1_sub_1_type);
	verify_type(relations.unique(), type_1_sub_1_sub_1_type);
	QVERIFY(relations.ambiguous().empty());
}

void type_relations_factory_test::should_create_ambiguous_relations_for_the_same_type()
{
	auto relations = type_relations_factory{}.create_type_relations({type_1_type, type_1_type});

	QVERIFY(relations.unique().empty());
	QCOMPARE(relations.ambiguous().size(), 1ul);
	verify_type(relations.ambiguous(), type_1_type);
}

void type_relations_factory_test::should_create_mixed_relations_for_subtypes()
{
	auto relations = type_relations_factory{}.create_type_relations({type_1_sub_1_sub_1_type, type_1_sub_1_sub_2_type});

	QCOMPARE(relations.unique().size(), 2ul);
	verify_type(relations.unique(), type_1_sub_1_sub_1_type);
	verify_type(relations.unique(), type_1_sub_1_sub_2_type);
	QCOMPARE(relations.ambiguous().size(), 2ul);
	verify_type(relations.ambiguous(), type_1_type);
	verify_type(relations.ambiguous(), type_1_sub_1_type);
}

void type_relations_factory_test::should_create_mixed_relations_for_type_and_subtype()
{
	auto relations = type_relations_factory{}.create_type_relations({type_1_type, type_1_sub_1_sub_1_type});

	QCOMPARE(relations.unique().size(), 2ul);
	verify_type(relations.unique(), type_1_sub_1_type);
	verify_type(relations.unique(), type_1_sub_1_sub_1_type);
	QCOMPARE(relations.ambiguous().size(), 1ul);
	verify_type(relations.ambiguous(), type_1_type);
}

void type_relations_factory_test::should_create_mixed_relations_for_subtype_and_type()
{
	auto relations = type_relations_factory{}.create_type_relations({type_1_sub_1_sub_1_type, type_1_type});

	QCOMPARE(relations.unique().size(), 2ul);
	verify_type(relations.unique(), type_1_sub_1_type);
	verify_type(relations.unique(), type_1_sub_1_sub_1_type);
	QCOMPARE(relations.ambiguous().size(), 1ul);
}

QTEST_APPLESS_MAIN(type_relations_factory_test);

#include "type-relations-factory-test.moc"
