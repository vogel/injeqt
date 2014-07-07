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
#include "dependency-apply-method.h"
#include "dependency-resolver.cpp"
#include "implements-extractor.cpp"
#include "injeqt-object.cpp"
#include "meta-object.cpp"
#include "meta-object-factory.cpp"
#include "resolved-dependency.cpp"

#include <QtCore/QMetaObject>
#include <QtTest/QtTest>

using namespace injeqt::v1;

class injectable_type1 : public QObject
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

class sublcass_injectable_type1 : public injectable_type1
{
	Q_OBJECT
};

class dependency_resolver_test : public QObject
{
	Q_OBJECT

private slots:
	void should_resolve_simple_dependency();
	void should_resolve_subclass_dependency();
	void should_resolve_to_first_matching_dependency();
	void should_resolve_to_first_matching_subclass_dependency();
	void should_not_resolve_superclass_dependency();
	void should_not_resolve_unmatching_dependency();
	void should_resolve_no_dependencies_when_no_objects_available();
	void should_resolve_all_dependencies();
	void should_resolve_available_dependencies();
	void should_resolve_available_dependencies_using_first_matching();
	void should_resolve_available_dependencies_using_first_matching_subclass();
	void should_resolve_available_dependencies_not_using_superclass();

private:
	template<typename T>
	injeqt_object make_injeqt_object();

};

template<typename T>
injeqt_object dependency_resolver_test::make_injeqt_object()
{
	auto qobject = std::unique_ptr<QObject>(new T{});
	auto object = injeqt_object
	{
		meta_object_factory{}.create_meta_object(T::staticMetaObject),
		std::move(qobject)
	};

	return object;
}

void dependency_resolver_test::should_resolve_simple_dependency()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto object2 = make_injeqt_object<injectable_type2>();
	auto object3 = make_injeqt_object<injectable_type3>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(object1),
		std::addressof(object2),
		std::addressof(object3)
	};
	auto to_resolve = dependency{
		injectable_type1::staticMetaObject,
		dependency_apply_method::setter,
		QMetaMethod{}
	};

	auto resolved = dependency_resolver{}.resolve_dependency(to_resolve, objects);
	QCOMPARE(resolved, std::addressof(object1));
}

void dependency_resolver_test::should_resolve_subclass_dependency()
{
	auto object1 = make_injeqt_object<sublcass_injectable_type1>();
	auto object2 = make_injeqt_object<injectable_type2>();
	auto object3 = make_injeqt_object<injectable_type3>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(object1),
		std::addressof(object2),
		std::addressof(object3),
	};
	auto to_resolve = dependency{
		injectable_type1::staticMetaObject,
		dependency_apply_method::setter,
		QMetaMethod{}
	};

	auto resolved = dependency_resolver{}.resolve_dependency(to_resolve, objects);
	QCOMPARE(resolved, std::addressof(object1));
}

void dependency_resolver_test::should_resolve_to_first_matching_dependency()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto object1b = make_injeqt_object<injectable_type1>();
	auto object2 = make_injeqt_object<injectable_type2>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(object1),
		std::addressof(object2),
		std::addressof(object1b)
	};
	auto to_resolve = dependency{
		injectable_type1::staticMetaObject,
		dependency_apply_method::setter,
		QMetaMethod{}
	};

	auto resolved = dependency_resolver{}.resolve_dependency(to_resolve, objects);
	QCOMPARE(resolved, std::addressof(object1));
	QVERIFY(std::addressof(object1) != std::addressof(object1b));
}

void dependency_resolver_test::should_resolve_to_first_matching_subclass_dependency()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto subobject1 = make_injeqt_object<sublcass_injectable_type1>();
	auto object2 = make_injeqt_object<injectable_type2>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(subobject1),
		std::addressof(object1),
		std::addressof(object2)
	};
	auto to_resolve = dependency{
		injectable_type1::staticMetaObject,
		dependency_apply_method::setter,
		QMetaMethod{}
	};

	auto resolved = dependency_resolver{}.resolve_dependency(to_resolve, objects);
	QCOMPARE(resolved, std::addressof(subobject1));
	QVERIFY(std::addressof(object1) != std::addressof(subobject1));
}

void dependency_resolver_test::should_not_resolve_superclass_dependency()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto object2 = make_injeqt_object<injectable_type2>();
	auto object3 = make_injeqt_object<injectable_type3>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(object1),
		std::addressof(object2),
		std::addressof(object3),
	};
	auto to_resolve = dependency{
		sublcass_injectable_type1::staticMetaObject,
		dependency_apply_method::setter,
		QMetaMethod{}
	};

	auto resolved = dependency_resolver{}.resolve_dependency(to_resolve, objects);
	QCOMPARE(resolved, static_cast<injeqt_object *>(nullptr));
}

void dependency_resolver_test::should_not_resolve_unmatching_dependency()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto object2 = make_injeqt_object<injectable_type2>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(object1),
		std::addressof(object2),
	};
	auto to_resolve = dependency{
		injectable_type3::staticMetaObject,
		dependency_apply_method::setter,
		QMetaMethod{}
	};

	auto resolved = dependency_resolver{}.resolve_dependency(to_resolve, objects);
	QCOMPARE(resolved, static_cast<injeqt_object *>(nullptr));
}

void dependency_resolver_test::should_resolve_no_dependencies_when_no_objects_available()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto object2 = make_injeqt_object<injectable_type2>();
	auto object3 = make_injeqt_object<injectable_type3>();
	auto objects = std::vector<const injeqt_object *>{};
	auto to_resolve = std::vector<dependency>{
		{
			injectable_type1::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type2::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type3::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		}
	};

	auto result = dependency_resolver{}.resolve_dependencies(to_resolve, objects);
	QVERIFY(result.resolved.empty());
	QCOMPARE(result.unresolved, to_resolve);
}

void dependency_resolver_test::should_resolve_all_dependencies()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto object2 = make_injeqt_object<injectable_type2>();
	auto object3 = make_injeqt_object<injectable_type3>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(object1),
		std::addressof(object2),
		std::addressof(object3),
	};
	auto to_resolve = std::vector<dependency>{
		{
			injectable_type1::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type2::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type3::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		}
	};

	auto result = dependency_resolver{}.resolve_dependencies(to_resolve, objects);
	QCOMPARE(result.resolved.size(), 3ul);
	QCOMPARE(result.resolved.at(0), (resolved_dependency{to_resolve.at(0), object1}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{to_resolve.at(1), object2}));
	QCOMPARE(result.resolved.at(2), (resolved_dependency{to_resolve.at(2), object3}));
	QVERIFY(result.unresolved.empty());
}

void dependency_resolver_test::should_resolve_available_dependencies()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto object3 = make_injeqt_object<injectable_type3>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(object1),
		std::addressof(object3),
	};
	auto to_resolve = std::vector<dependency>{
		{
			injectable_type1::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type2::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type3::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		}
	};

	auto result = dependency_resolver{}.resolve_dependencies(to_resolve, objects);
	QCOMPARE(result.resolved.size(), 2ul);
	QCOMPARE(result.resolved.at(0), (resolved_dependency{to_resolve.at(0), object1}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{to_resolve.at(2), object3}));
	QCOMPARE(result.unresolved.size(), 1ul);
	QCOMPARE(result.unresolved.at(0), to_resolve.at(1));
}

void dependency_resolver_test::should_resolve_available_dependencies_using_first_matching()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto object1b = make_injeqt_object<injectable_type1>();
	auto object3 = make_injeqt_object<injectable_type3>();
	auto object3b = make_injeqt_object<injectable_type3>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(object1),
		std::addressof(object1b),
		std::addressof(object3),
		std::addressof(object3b),
	};
	auto to_resolve = std::vector<dependency>{
		{
			injectable_type1::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type2::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type3::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		}
	};

	auto result = dependency_resolver{}.resolve_dependencies(to_resolve, objects);
	QCOMPARE(result.resolved.size(), 2ul);
	QCOMPARE(result.resolved.at(0), (resolved_dependency{to_resolve.at(0), object1}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{to_resolve.at(2), object3}));
	QCOMPARE(result.unresolved.size(), 1ul);
	QCOMPARE(result.unresolved.at(0), to_resolve.at(1));
	QVERIFY(std::addressof(object1) != std::addressof(object1b));
	QVERIFY(std::addressof(object3) != std::addressof(object3b));
}

void dependency_resolver_test::should_resolve_available_dependencies_using_first_matching_subclass()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto subobject1 = make_injeqt_object<sublcass_injectable_type1>();
	auto object3 = make_injeqt_object<injectable_type3>();
	auto object3b = make_injeqt_object<injectable_type3>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(subobject1),
		std::addressof(object1),
		std::addressof(object3),
		std::addressof(object3b),
	};
	auto to_resolve = std::vector<dependency>{
		{
			injectable_type1::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type2::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type3::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		}
	};

	auto result = dependency_resolver{}.resolve_dependencies(to_resolve, objects);
	QCOMPARE(result.resolved.size(), 2ul);
	QCOMPARE(result.resolved.at(0), (resolved_dependency{to_resolve.at(0), subobject1}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{to_resolve.at(2), object3}));
	QCOMPARE(result.unresolved.size(), 1ul);
	QCOMPARE(result.unresolved.at(0), to_resolve.at(1));
	QVERIFY(std::addressof(object1) != std::addressof(subobject1));
	QVERIFY(std::addressof(object3) != std::addressof(object3b));
}

void dependency_resolver_test::should_resolve_available_dependencies_not_using_superclass()
{
	auto object1 = make_injeqt_object<injectable_type1>();
	auto object2 = make_injeqt_object<injectable_type2>();
	auto objects = std::vector<const injeqt_object *>{
		std::addressof(object1),
		std::addressof(object2)
	};
	auto to_resolve = std::vector<dependency>{
		{
			sublcass_injectable_type1::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		},
		{
			injectable_type2::staticMetaObject,
			dependency_apply_method::setter,
			QMetaMethod{}
		}
	};

	auto result = dependency_resolver{}.resolve_dependencies(to_resolve, objects);
	QCOMPARE(result.resolved.size(), 1ul);
	QCOMPARE(result.resolved.at(0), (resolved_dependency{to_resolve.at(1), object2}));
	QCOMPARE(result.unresolved.size(), 1ul);
	QCOMPARE(result.unresolved.at(0), to_resolve.at(0));
}

QTEST_APPLESS_MAIN(dependency_resolver_test);

#include "dependency-resolver-test.moc"
