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

#include "dependencies.cpp"
#include "dependency.cpp"
#include "dependency-extractor.cpp"
#include "implements-extractor.cpp"
#include "setter-method.cpp"
#include "type.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>

using namespace injeqt::v1;

class injectable_type1 : public QObject
{
	Q_OBJECT
};

class sub_injectable_type1a : public injectable_type1
{
	Q_OBJECT
};

class sub_injectable_type1b : public injectable_type1
{
	Q_OBJECT
};

class injectable_type2 : public QObject
{
	Q_OBJECT
};

class injectable_type3 : public QObject
{
	Q_OBJECT
};

class valid_injected_type : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void setter_1(injectable_type1 *) {}
	injeqt_setter void setter_2(injectable_type2 *) {}
	void invalid_setter_1(injectable_type1 *) {}
	void invalid_setter_2(injectable_type2 *) {}
	void invalid_setter_3(int) {}

};

class inheriting_valid_injected_type : public valid_injected_type
{
	Q_OBJECT

public slots:
	injeqt_setter void setter_3(injectable_type3 *) {}

};

class valid_injected_type_with_common_superclass : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void setter_1(sub_injectable_type1a *) {}
	injeqt_setter void setter_2(sub_injectable_type1b *) {}

};

class too_many_parameters_invalid_injected_type : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void setter_1(injectable_type1 *, injectable_type2 *) {}

};

class non_qobject_invalid_injected_type : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void setter_1(int) {}

};

class duplicate_dependency_invalid_injected_type : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void setter_1(injectable_type1 *) {}
	injeqt_setter void setter_2(injectable_type1 *) {}

};

class invalid_injected_type_with_superclass : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void setter_1(injectable_type1 *) {}
	injeqt_setter void setter_2(sub_injectable_type1a *) {}

};

class invalid_injected_type_with_superclass_inverted : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void setter_2(sub_injectable_type1a *) {}
	injeqt_setter void setter_1(injectable_type1 *) {}

};

class dependency_extractor_test : public QObject
{
	Q_OBJECT

public:
	dependency_extractor_test();

private slots:
	void should_find_all_valid_dependencies();
	void should_find_all_valid_dependencies_in_hierarchy();
	void should_find_dependencies_with_common_superclass();
	void should_fail_when_too_many_parameters();
	void should_fail_when_type_not_qobject();
	void should_fail_when_duplicate_dependency();
	void should_fail_with_superclass_dependency();
	void should_fail_with_superclass_inverted_dependency();

private:
	type injectable_type1_type;
	type injectable_type2_type;
	type injectable_type3_type;
	type valid_injected_type_type;
	type inheriting_valid_injected_type_type;
	type valid_injected_type_with_common_superclass_type;
	type too_many_parameters_invalid_injected_type_type;
	type non_qobject_invalid_injected_type_type;
	type duplicate_dependency_invalid_injected_type_type;
	type invalid_injected_type_with_superclass_type;
	type invalid_injected_type_with_superclass_inverted_type;
	setter_method valid_injected_type_setter_1;
	setter_method valid_injected_type_setter_2;
	setter_method valid_injected_type_with_common_superclass_setter_1;
	setter_method valid_injected_type_with_common_superclass_setter_2;
	setter_method inheriting_valid_injected_type_setter_1;
	setter_method inheriting_valid_injected_type_setter_2;
	setter_method inheriting_valid_injected_type_setter_3;
	setter_method invalid_injected_type_with_superclass_setter_1;
	setter_method invalid_injected_type_with_superclass_setter_2;
	setter_method invalid_injected_type_with_superclass_inverted_setter_1;
	setter_method invalid_injected_type_with_superclass_inverted_setter_2;

	void verify_dependency(dependencies list, const dependency &check);

};

dependency_extractor_test::dependency_extractor_test() :
	injectable_type1_type{std::addressof(injectable_type1::staticMetaObject)},
	injectable_type2_type{std::addressof(injectable_type2::staticMetaObject)},
	injectable_type3_type{std::addressof(injectable_type3::staticMetaObject)},
	valid_injected_type_type{std::addressof(valid_injected_type::staticMetaObject)},
	inheriting_valid_injected_type_type{std::addressof(inheriting_valid_injected_type::staticMetaObject)},
	valid_injected_type_with_common_superclass_type{std::addressof(valid_injected_type_with_common_superclass::staticMetaObject)},
	too_many_parameters_invalid_injected_type_type{std::addressof(too_many_parameters_invalid_injected_type::staticMetaObject)},
	non_qobject_invalid_injected_type_type{std::addressof(non_qobject_invalid_injected_type::staticMetaObject)},
	duplicate_dependency_invalid_injected_type_type{std::addressof(duplicate_dependency_invalid_injected_type::staticMetaObject)},
	invalid_injected_type_with_superclass_type{std::addressof(invalid_injected_type_with_superclass::staticMetaObject)},
	invalid_injected_type_with_superclass_inverted_type{std::addressof(invalid_injected_type_with_superclass_inverted::staticMetaObject)},
	valid_injected_type_setter_1{method<valid_injected_type>("setter_1(injectable_type1*)")},
	valid_injected_type_setter_2{method<valid_injected_type>("setter_2(injectable_type2*)")},
	valid_injected_type_with_common_superclass_setter_1{method<valid_injected_type_with_common_superclass>("setter_1(sub_injectable_type1a*)")},
	valid_injected_type_with_common_superclass_setter_2{method<valid_injected_type_with_common_superclass>("setter_2(sub_injectable_type1b*)")},
	inheriting_valid_injected_type_setter_1{method<valid_injected_type>("setter_1(injectable_type1*)")},
	inheriting_valid_injected_type_setter_2{method<valid_injected_type>("setter_2(injectable_type2*)")},
	inheriting_valid_injected_type_setter_3{method<inheriting_valid_injected_type>("setter_3(injectable_type3*)")},
	invalid_injected_type_with_superclass_setter_1{method<invalid_injected_type_with_superclass>("setter_1(injectable_type1*)")},
	invalid_injected_type_with_superclass_setter_2{method<invalid_injected_type_with_superclass>("setter_2(sub_injectable_type1a*)")},
	invalid_injected_type_with_superclass_inverted_setter_1{method<invalid_injected_type_with_superclass_inverted>("setter_1(injectable_type1*)")},
	invalid_injected_type_with_superclass_inverted_setter_2{method<invalid_injected_type_with_superclass_inverted>("setter_2(sub_injectable_type1a*)")}
{
}

void dependency_extractor_test::verify_dependency(dependencies list, const dependency &check)
{
	auto iterator = std::find(begin(list), end(list), check);
	QVERIFY(iterator != end(list));
}

void dependency_extractor_test::should_find_all_valid_dependencies()
{
	auto dependencies = dependency_extractor{}.extract_dependencies(valid_injected_type_type);
	QCOMPARE(dependencies.size(), 2UL);
	verify_dependency(dependencies, dependency{valid_injected_type_setter_1});
	verify_dependency(dependencies, dependency{valid_injected_type_setter_2});
}

void dependency_extractor_test::should_find_all_valid_dependencies_in_hierarchy()
{
	auto dependencies = dependency_extractor{}.extract_dependencies(inheriting_valid_injected_type_type);
	QCOMPARE(dependencies.size(), 3UL);
	verify_dependency(dependencies, dependency{inheriting_valid_injected_type_setter_1});
	verify_dependency(dependencies, dependency{inheriting_valid_injected_type_setter_2});
	verify_dependency(dependencies, dependency{inheriting_valid_injected_type_setter_3});
}

void dependency_extractor_test::should_find_dependencies_with_common_superclass()
{
	auto dependencies = dependency_extractor{}.extract_dependencies(valid_injected_type_with_common_superclass_type);

	QCOMPARE(dependencies.size(), 2UL);
	verify_dependency(dependencies, dependency{valid_injected_type_with_common_superclass_setter_1});
	verify_dependency(dependencies, dependency{valid_injected_type_with_common_superclass_setter_2});
}

void dependency_extractor_test::should_fail_when_too_many_parameters()
{
	expect<dependency_too_many_parameters_exception>([&]{
		auto dependencies = dependency_extractor{}.extract_dependencies(too_many_parameters_invalid_injected_type_type);
	});
}

void dependency_extractor_test::should_fail_when_type_not_qobject()
{
	expect<dependency_not_qobject_exception>([&]{
		auto dependencies = dependency_extractor{}.extract_dependencies(non_qobject_invalid_injected_type_type);
	});
}

void dependency_extractor_test::should_fail_when_duplicate_dependency()
{
	expect<dependency_duplicated_exception>([&]{
		auto dependencies = dependency_extractor{}.extract_dependencies(duplicate_dependency_invalid_injected_type_type);
	});
}

void dependency_extractor_test::should_fail_with_superclass_dependency()
{
	expect<dependency_duplicated_exception>([&]{
		auto dependencies = dependency_extractor{}.extract_dependencies(invalid_injected_type_with_superclass_type);
	});
}

void dependency_extractor_test::should_fail_with_superclass_inverted_dependency()
{
	expect<dependency_duplicated_exception>([&]{
		auto dependencies = dependency_extractor{}.extract_dependencies(invalid_injected_type_with_superclass_inverted_type);
	});
}

QTEST_APPLESS_MAIN(dependency_extractor_test);

#include "dependency-extractor-test.moc"
