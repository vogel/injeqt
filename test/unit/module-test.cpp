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

#include "../mocks/mocked-provider.h"
#include "expect.h"
#include "utils.h"

#include <injeqt/module.h>

#include <QtTest/QtTest>
#include <string>

using namespace injeqt::internal;
using namespace injeqt::v1;

class not_default_constructible_type : public QObject
{
	Q_OBJECT
};

class not_default_constructible_type_subtype : public not_default_constructible_type
{
	Q_OBJECT
};

class default_constructible_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE default_constructible_type() {}
};

class factory_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE default_constructible_type * create() const { return nullptr; }
};

class factory_subtype_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE not_default_constructible_type_subtype * create() const { return nullptr; }
};

class factory_double_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE not_default_constructible_type * create1() const { return nullptr; }
	Q_INVOKABLE not_default_constructible_type * create2() const { return nullptr; }
};

class module_test : public QObject
{
	Q_OBJECT

private slots:
	void should_accept_empty_module();
	void should_accept_null_ready_object();
	void should_accept_qobject_ready_object();
	void should_accept_valid_ready_object();
	void should_accept_subtype_ready_object();
	void should_accept_supertype_ready_object();
	void should_accept_qobject_type();
	void should_accept_not_default_constructible_type();
	void should_accept_default_constructible_type();
	void should_accept_qobject_factory_type();
	void should_accept_qobject_created_type();
	void should_accept_valid_factory_type();
	void should_accept_supertype_factory_type();
	void should_accept_subtype_factory_type();
	void should_accept_double_factory_type();

};

void module_test::should_accept_empty_module()
{
	class test_module : public module { };

	test_module{};
}

void module_test::should_accept_null_ready_object()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_ready_object<not_default_constructible_type>(nullptr);
		}
	};

	test_module{};
}

void module_test::should_accept_qobject_ready_object()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			not_default_constructible_type object;
			add_ready_object<QObject>(&object);
		}
	};

	test_module{};
}

void module_test::should_accept_valid_ready_object()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			not_default_constructible_type object;
			add_ready_object<not_default_constructible_type>(&object);
		}
	};

	test_module{};
}

void module_test::should_accept_subtype_ready_object()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			not_default_constructible_type_subtype object;
			add_ready_object<not_default_constructible_type>(&object);
		}
	};

	test_module{};
}

void module_test::should_accept_supertype_ready_object()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			not_default_constructible_type object;
			add_ready_object<not_default_constructible_type_subtype>(&object);
		}
	};

	test_module{};
}

void module_test::should_accept_qobject_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_type<QObject>();
		}
	};

	test_module{};
}

void module_test::should_accept_not_default_constructible_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_type<not_default_constructible_type>();
		}
	};

	test_module{};
}

void module_test::should_accept_default_constructible_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_type<default_constructible_type>();
		}
	};

	test_module{};
}

void module_test::should_accept_qobject_factory_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<not_default_constructible_type, QObject>();
		}
	};

	test_module{};
}

void module_test::should_accept_qobject_created_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<QObject, factory_type>();
		}
	};

	test_module{};
}

void module_test::should_accept_valid_factory_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<not_default_constructible_type, factory_type>();
			add_factory<not_default_constructible_type_subtype, factory_subtype_type>();
		}
	};

	test_module{};
}

void module_test::should_accept_subtype_factory_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<not_default_constructible_type_subtype, factory_type>();
		}
	};

	test_module{};
}

void module_test::should_accept_supertype_factory_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<not_default_constructible_type, factory_subtype_type>();
		}
	};

	test_module{};
}

void module_test::should_accept_double_factory_type()
{
	class test_module : public module
	{
	public:
		test_module()
		{
			add_factory<not_default_constructible_type, factory_double_type>();
		}
	};

	test_module{};
}

QTEST_APPLESS_MAIN(module_test)
#include "module-test.moc"
