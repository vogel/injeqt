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

#include "dependency.cpp"
#include "implements-extractor.cpp"
#include "meta-object.cpp"
#include "meta-object-factory.cpp"
#include "object-with-meta.cpp"
#include "resolved-dependency.cpp"
#include "resolved-dependency-applicator.cpp"
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

class sub_injectable_type1 : public injectable_type1
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

public:
	injectable_type1 *_1 = nullptr;
	injectable_type2 *_2 = nullptr;

public slots:
	injeqt_setter void setter_1(injectable_type1 *a) { _1 = a; }
	injeqt_setter void setter_2(injectable_type2 *a) { _2 = a;}

};

class sub_valid_injected_type : public QObject
{
	Q_OBJECT

public:
	sub_injectable_type1 *_1 = nullptr;
	injectable_type2 *_2 = nullptr;

public slots:
	injeqt_setter void setter_1(sub_injectable_type1 *a) { _1 = a; }
	injeqt_setter void setter_2(injectable_type2 *a) { _2 = a;}

};

class valid_injected_type2 : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void setter_3(injectable_type1 *) {}
	injeqt_setter void setter_4(injectable_type2 *) {}

};

class resolved_dependency_applicator_test : public QObject
{
	Q_OBJECT

public:
	resolved_dependency_applicator_test();

private slots:
	void should_apply_nothing_when_empty_list_passed();
	void should_apply_one_dependency_when_one_passed();
	void should_apply_all_dependencies_when_all_passed();
	void should_throw_with_invalid_dependency();
	void should_throw_with_null_dependency();
	void should_throw_with_subclass_dependency();
	void should_throw_with_superclass_dependency();
	void should_throw_when_applying_on_wrong_object();

private:
	type valid_injected_type_type;
	type injectable_type1_type;
	type injectable_type2_type;
	type sub_injectable_type1_type;
	setter_method valid_injected_type_injectable_type1_setter;
	setter_method valid_injected_type_injectable_type2_setter;
	setter_method sub_valid_injected_type_sub_injectable_type1_setter;
};

resolved_dependency_applicator_test::resolved_dependency_applicator_test() :
	valid_injected_type_type{std::addressof(valid_injected_type::staticMetaObject)},
	injectable_type1_type{std::addressof(injectable_type1::staticMetaObject)},
	injectable_type2_type{std::addressof(injectable_type2::staticMetaObject)},
	sub_injectable_type1_type{std::addressof(sub_injectable_type1::staticMetaObject)},
	valid_injected_type_injectable_type1_setter{method<valid_injected_type>("setter_1(injectable_type1*)")},
	valid_injected_type_injectable_type2_setter{method<valid_injected_type>("setter_2(injectable_type2*)")},
	sub_valid_injected_type_sub_injectable_type1_setter{method<sub_valid_injected_type>("setter_1(sub_injectable_type1*)")}
{
}

void resolved_dependency_applicator_test::should_apply_nothing_when_empty_list_passed()
{
	auto resolved_dependencies = std::vector<resolved_dependency>{};
	auto injected_object = make_object_with_meta<valid_injected_type>();
	auto applicator = resolved_dependency_applicator{resolved_dependencies};
	applicator.apply_on(injected_object);

	QCOMPARE(injected_object.object_as<valid_injected_type>()->_1, static_cast<QObject *>(nullptr));
	QCOMPARE(injected_object.object_as<valid_injected_type>()->_2, static_cast<QObject *>(nullptr));
}

void resolved_dependency_applicator_test::should_apply_one_dependency_when_one_passed()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto resolved_dependencies = std::vector<resolved_dependency>
	{
		resolved_dependency{dependency{valid_injected_type_injectable_type1_setter}, object1}
	};
	auto injected_object = make_object_with_meta<valid_injected_type>();
	auto applicator = resolved_dependency_applicator{resolved_dependencies};
	applicator.apply_on(injected_object);

	QCOMPARE(injected_object.object_as<valid_injected_type>()->_1, object1.object());
	QCOMPARE(injected_object.object_as<valid_injected_type>()->_2, static_cast<QObject *>(nullptr));
}

void resolved_dependency_applicator_test::should_apply_all_dependencies_when_all_passed()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto object2 = make_object_with_meta<injectable_type2>();
	auto resolved_dependencies = std::vector<resolved_dependency>
	{
		resolved_dependency{dependency{valid_injected_type_injectable_type1_setter}, object1},
		resolved_dependency{dependency{valid_injected_type_injectable_type2_setter}, object2}
	};
	auto injected_object = make_object_with_meta<valid_injected_type>();
	auto applicator = resolved_dependency_applicator{resolved_dependencies};
	applicator.apply_on(injected_object);

	QCOMPARE(injected_object.object_as<valid_injected_type>()->_1, object1.object());
	QCOMPARE(injected_object.object_as<valid_injected_type>()->_2, object2.object());
}

void resolved_dependency_applicator_test::should_throw_with_null_dependency()
{
	auto object = object_with_meta
	{
		meta_object_factory{}.create_meta_object(injectable_type1_type),
		nullptr
	};
	auto resolved_dependencies = std::vector<resolved_dependency>
	{
		resolved_dependency{dependency{valid_injected_type_injectable_type2_setter}, object}
	};
	auto injected_object = make_object_with_meta<valid_injected_type>();
	expect<applicator_invalid_dependency_exception>([&]{
		auto applicator = resolved_dependency_applicator{resolved_dependencies};
	});
}

void resolved_dependency_applicator_test::should_throw_with_subclass_dependency()
{
	auto sub_object1 = make_object_with_meta<sub_injectable_type1>();
	auto resolved_dependencies = std::vector<resolved_dependency>
	{
		resolved_dependency{dependency{valid_injected_type_injectable_type1_setter}, sub_object1}
	};
	auto injected_object = make_object_with_meta<valid_injected_type>();
	expect<applicator_invalid_dependency_exception>([&]{
		auto applicator = resolved_dependency_applicator{resolved_dependencies};
	});
}

void resolved_dependency_applicator_test::should_throw_with_superclass_dependency()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto resolved_dependencies = std::vector<resolved_dependency>
	{
		resolved_dependency{dependency{sub_valid_injected_type_sub_injectable_type1_setter}, object1}
	};
	auto injected_object = make_object_with_meta<sub_valid_injected_type>();
	expect<applicator_invalid_dependency_exception>([&]{
		auto applicator = resolved_dependency_applicator{resolved_dependencies};
	});
}

void resolved_dependency_applicator_test::should_throw_with_invalid_dependency()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto resolved_dependencies = std::vector<resolved_dependency>
	{
		resolved_dependency{dependency{valid_injected_type_injectable_type2_setter}, object1}
	};
	auto injected_object = make_object_with_meta<valid_injected_type>();
	expect<applicator_invalid_dependency_exception>([&]{
		auto applicator = resolved_dependency_applicator{resolved_dependencies};
	});
}

void resolved_dependency_applicator_test::should_throw_when_applying_on_wrong_object()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto object2 = make_object_with_meta<injectable_type2>();
	auto resolved_dependencies = std::vector<resolved_dependency>
	{
		resolved_dependency{dependency{valid_injected_type_injectable_type1_setter}, object1},
		resolved_dependency{dependency{valid_injected_type_injectable_type2_setter}, object2}
	};
	auto injected_object = make_object_with_meta<valid_injected_type2>();
	auto applicator = resolved_dependency_applicator{resolved_dependencies};
	expect<applicator_non_matching_setter_exception>([&]{
		applicator.apply_on(injected_object);
	});
}

QTEST_APPLESS_MAIN(resolved_dependency_applicator_test);

#include "resolved-dependency-applicator-test.moc"
