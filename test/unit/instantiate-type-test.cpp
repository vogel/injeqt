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

#include "expect.h"
#include "utils.h"

#include <injeqt/exception/invalid-instantiate-type.h>

#include "internal/instantiate-type.h"

#include <QtTest/QtTest>
#include <string>

using namespace injeqt::internal;
using namespace injeqt::v1;

class default_instantiate_type : public QObject
{
	Q_OBJECT
};

class on_demand_instantiate_type : public default_instantiate_type
{
	Q_OBJECT
	INJEQT_INSTANCE_ON_DEMAND
};

class on_demand_instantiate_inherited_type : public on_demand_instantiate_type
{
	Q_OBJECT
};

class on_demand_twice_instantiate_type : public default_instantiate_type
{
	Q_OBJECT
	INJEQT_INSTANCE_ON_DEMAND
	INJEQT_INSTANCE_ON_DEMAND
};

class immediate_inherited_from_on_demand_type : public on_demand_instantiate_type
{
	Q_OBJECT
	INJEQT_INSTANCE_IMMEDIATE
};

class immediate_type : public default_instantiate_type
{
	Q_OBJECT
	INJEQT_INSTANCE_IMMEDIATE
};

class immediate_inherited_type : public immediate_type
{
	Q_OBJECT
};

class immediate_twice_instantiate_type : public default_instantiate_type
{
	Q_OBJECT
	INJEQT_INSTANCE_IMMEDIATE
	INJEQT_INSTANCE_IMMEDIATE
};

class on_demand_inherited_from_immediate_type : public immediate_type
{
	Q_OBJECT
	INJEQT_INSTANCE_ON_DEMAND
};

class mixed_two_instantiate_type : public default_instantiate_type
{
	Q_OBJECT
	INJEQT_INSTANCE_IMMEDIATE
	INJEQT_INSTANCE_ON_DEMAND
};

class invalid_instantiate_type : public default_instantiate_type
{
	Q_OBJECT
	Q_CLASSINFO(INJEQT_INSTANTIATE_CLASSINFO_NAME, "invalid")
};

class instantiate_type_test : public QObject
{
	Q_OBJECT

private slots:
	void should_get_on_demand_by_default();
	void should_get_on_demand_when_on_demand_requested();
	void should_get_on_demand_when_on_demand_requested_in_supertype();
	void should_get_on_demand_when_on_demand_requested_twice();
	void should_get_immediate_when_override_supertype();
	void should_get_immediate_when_immediate_requested();
	void should_get_immediate_when_immediate_requested_in_supertype();
	void should_get_immediate_when_immediate_requested_twice();
	void should_get_on_demand_when_override_supertype();
	void should_get_second_when_two_different();
	void should_throw_when_invalid();

};

void instantiate_type_test::should_get_on_demand_by_default()
{
	QCOMPARE(instantiate_type::on_demand, get_instantiate_type<default_instantiate_type>());
}

void instantiate_type_test::should_get_on_demand_when_on_demand_requested()
{
	QCOMPARE(instantiate_type::on_demand, get_instantiate_type<on_demand_instantiate_type>());
}

void instantiate_type_test::should_get_on_demand_when_on_demand_requested_in_supertype()
{
	QCOMPARE(instantiate_type::on_demand, get_instantiate_type<on_demand_instantiate_inherited_type>());
}

void instantiate_type_test::should_get_on_demand_when_on_demand_requested_twice()
{
	QCOMPARE(instantiate_type::on_demand, get_instantiate_type<on_demand_twice_instantiate_type>());
}

void instantiate_type_test::should_get_immediate_when_override_supertype()
{
	QCOMPARE(instantiate_type::immediate, get_instantiate_type<immediate_inherited_from_on_demand_type>());
}

void instantiate_type_test::should_get_immediate_when_immediate_requested()
{
	QCOMPARE(instantiate_type::immediate, get_instantiate_type<immediate_type>());
}

void instantiate_type_test::should_get_immediate_when_immediate_requested_in_supertype()
{
	QCOMPARE(instantiate_type::immediate, get_instantiate_type<immediate_inherited_type>());
}

void instantiate_type_test::should_get_immediate_when_immediate_requested_twice()
{
	QCOMPARE(instantiate_type::immediate, get_instantiate_type<immediate_twice_instantiate_type>());
}

void instantiate_type_test::should_get_on_demand_when_override_supertype()
{
	QCOMPARE(instantiate_type::on_demand, get_instantiate_type<on_demand_inherited_from_immediate_type>());
}

void instantiate_type_test::should_get_second_when_two_different()
{
	QCOMPARE(instantiate_type::on_demand, get_instantiate_type<mixed_two_instantiate_type>());
}

void instantiate_type_test::should_throw_when_invalid()
{
	expect<exception::invalid_instantiate_type>([&] {
		get_instantiate_type<invalid_instantiate_type>();
	});
}

QTEST_APPLESS_MAIN(instantiate_type_test)
#include "instantiate-type-test.moc"
