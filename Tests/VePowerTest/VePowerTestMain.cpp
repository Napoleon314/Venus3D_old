////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//	Module:      VePowerTest
//  File name:   VePowerTestMain.cpp
//  Created:     2015/02/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include <VePowerPch.h>
#ifdef VE_PLATFORM_WIN
#	include<vld.h>
#endif

class A : public VeMemObject
{
public:
	VeRTTIDecl(A);

	virtual ~A() {}

	VeUInt32 a = 5;

};

VeRTTIImpl(A);

class B : public VeMemObject
{
public:
	VeRTTIDecl(B);

	virtual ~B() {}

	VeUInt32 b = 8;

};

VeRTTIImpl(B);

class C : public A, public B
{
public:
	VeRTTIDecl(C, A, B);

	virtual ~C() {}

	VeUInt32 c = 11;

};

VeRTTIImpl(C, A, B);

class Test : public VeRefObject
{
public:

	VeInt32 m_i32Test = 5;
};

VeSmartPointer(Test);

#include <vector>
//#include <stdexcept>
//int posix_memalign(void **memptr, size_t alignment, size_t size);

void RefListTest();
void ResourceTest();
void LuaTest();

VeInt32 main(VeInt32 i32Argc, VeChar8* apcArgv[])
{
	/*bool bDebug = false;
	if (i32Argc > 1)
	{
		VeChar8** ppcArgv = apcArgv;
		if (VeStrcmp(*(apcArgv + 1), "-dlua") == 0)
		{
			bDebug = true;
			++ppcArgv;
		}
	}*/
	VeConsoleInit("com.VenusIE.VePowerTest");

	{
		C* pkC = VE_NEW C;
		A* pkA = static_cast<A*>(pkC);
		B* pkB = static_cast<B*>(pkC);


		bool b = VeIsKindOf(B, pkA);
		b = VeIsExactKindOf(B, pkA);


		/*VeSizeT stOffset = VeStd::base_offset<B,C>();

		VeSizeT stTemp = B::ms_RTTI.GetPathFrom(&C::ms_kRTTI);*/

		A* pkA2 = VeDynamicCast(A, pkB);
		B* pkB2 = VeDynamicCast(B, pkB);
		C* pkC2 = VeDynamicCast(C, pkC);
		C* pkC3 = VeDynamicCast(C, pkB);

		VeDebugOutput("%d,%d,%d,%d,%d", pkA2->a, pkB2->b, pkC2->c, pkC3->c, b);

		//VeStd::pair<VeInt32, VeInt32> kPair = VeStd::make_pair(3, 5);

		//A::GetPath

		/*VeRTTI a("A");
		VeRTTI b("B");
		VeRTTI c("C");

		VeRTTI d("D", &a, 0, &b, 4, &c, 8);

		for(auto&& it : d)
		{
		printf("%s,%d\n", it.first->GetName(), VeUInt32(it.second));
		}*/

		//std::vector<int> stda;
		/*stda.push_back(5);
		stda.push_back(6);
		stda.push_back(7);*/

		VE_DELETE(pkC);

		TestPtr spTest = VE_NEW Test;
		spTest = nullptr;
		//std::auto_ptr

		//VeChar8* pcName = "TestName";
		//VeSizeT stNum = strlen(pcName);

		std::vector<int> vec1;
		vec1.push_back(4);
		vec1.push_back(5);
		vec1.push_back(6);

		for (auto i : vec1)
		{
			VeDebugOutput("%d", i);
		}

		for (auto it = vec1.rbegin(); it != vec1.rend(); ++it)
		{
			VeDebugOutput("%d", *it);
		}

		VeInt32 test = VeStrcmp("abce", "abcd");
		VeDebugOutput("%d", test);

		//printf("我们\n");

		//VeLog::GetSingleton().LogFormat(VeLog::TYPE_DEBUG, "Venus3D", "%d,%d,%s", 8, 9, "aa");

		//g_pLog->Log(VeLog::TYPE_DEBUG, "Venus3D", 5.5f, "bb", 32, 4.5);

		VeDebugOutput("%d,%d,%s", 8, 9, "aa");

		VeCoreLogD(8, 9, "aa", "我们");

		//ve_lua.DoString("print(\"Hello Lua!\")");

		VeList<VeInt32> list;
		list.push_back(98);
		list.push_back(99);
		list.push_back(100);
		for (auto i : list)
		{
			VeDebugOutput("%d", i);
		}

		RefListTest();
		ResourceTest();
		//LuaTest();
	}

	VeConsoleTerm();
	return 0;
}

void RefListTest()
{
	typedef VeRefNode<int> Node;
	typedef VeRefList<int> List;
	List kLink;
	Node kNode1; kNode1.m_Content = 1;
	Node kNode2; kNode2.m_Content = 2;
	Node kNode3; kNode3.m_Content = 3;
	Node kNode4; kNode4.m_Content = 4;


	kLink.attach_back(kNode1);///<1>
	kLink.attach_after(kNode1, kNode2);///<1, 2>
	kLink.attach_front(kNode3);///<3, 1, 2>
	kLink.attach_before(kNode3, kNode4);///<4, 3, 1, 2>

	for (auto i : kLink.r())
	{
		VeCoreLogD(i);
		//kNode3.detach();
	}///<2,1,3,4>

	for (auto it = kLink.begin(); it != kLink.end(); ++it)
	{
		auto i = *it;
		VeCoreLogD(i);
		kNode2.detach();
	}///<4,3,1>

	for (auto i : kLink)
	{
		VeCoreLogD(i);
		kNode3.detach();
	}///<4,1>

	//struct Test
	//{
	//	Test(int _1, int _2) : a(_1), b(_2) {}
	//	int a, b;

	//	VeRefNode<venus::member_cast<Test,&Test::node>> node;
	//};


}



void ResourceTest()
{
	/*VeFilePathPtr spPath = VE_NEW VeFilePath("RunEnv");
	VeVector<VeString> kList;
	spPath->FindFileList("*", kList);
	for (auto i : kList)
	{
		VeDebugOutput(i.c_str());
	}

	struct TestRead : public VeRefObject
	{
		TestRead()
		{
			IncRefCount();
			m_kNode.m_Content = this;
			ve_sys.Collect(m_kNode);
			m_kTaskNode.m_Content = [](const VeMemoryIStreamPtr& spData) noexcept
			{
				const VeChar8* pcText = (const VeChar8*)(void*)(*(spData->GetBlob()));
				VeDebugOutput(pcText);
			};
			
		}

		VeRefNode<VeRefObject*> m_kNode;
		VeRefNode<VeDirectory::ReadCallback> m_kTaskNode;
	};

	if (spPath->HasFile("a.txt"))
	{
		TestRead* pkTest = VE_NEW TestRead();
		spPath->ReadAsync("a.txt", pkTest->m_kTaskNode);
		while (ve_res_mgr.HasTask())
		{
			ve_res_mgr.Update();
		}
	}

	VeMemoryOStreamPtr spStream = VE_NEW VeMemoryOStream;
	(*spStream) << 'a';
	(*spStream) << 'b';

	struct TestWrite : public VeRefObject
	{
		TestWrite()
		{
			IncRefCount();
			m_kNode.m_Content = this;
			ve_sys.Collect(m_kNode);
			m_kTaskNode.m_Content = [](VeResult eRes) noexcept
			{
				if (VE_SUCCEEDED(eRes))
				{
					VeDebugOutput("Write succeeded");
				}
				else
				{
					VeDebugOutput("Write failed");
				}

			};

		}

		VeRefNode<VeRefObject*> m_kNode;
		VeRefNode<VeDirectory::WriteCallback> m_kTaskNode;
	};

	{
		TestWrite* pkTest = VE_NEW TestWrite();
		spPath->WriteAsync("b.txt", spStream, pkTest->m_kTaskNode);
		while (ve_res_mgr.HasTask())
		{
			ve_res_mgr.Update();
		}
	}*/

	//VeDirectoryPtr spDir = VeFilePath::Create("RunEnv/aa/bb/cc");

	//bool bRes = VeFilePath::CreatePath("/sdcard");

	/*VeDirectoryPtr spDir = ve_res_mgr.CreateDir("RunEnv");
	if (spDir)
	{
		VeDebugOutput("CreateDir succeeded");
		VeVector<VeFixedString> kList;
		spDir->FindFileList("*", kList);
		for (auto& str : kList)
		{
			VeDebugOutput(str);
		}

		if (spDir->HasFile("a.txt"))
		{
			VeDebugOutput("Has a.txt");
		}
		else
		{
			VeDebugOutput("Has not a.txt");
		}
	}
	else
	{
		VeDebugOutput("CreateDir failed");
	}*/
	//VeFixedString kStr;
	//VeFixedString kStr2 = "abc";
	//VeFixedString kStr3 = kStr2;
	//VeFixedString kStr4 = kStr3;

}

//using namespace venus;
//
//VeInt32 TestFunc(VeInt32 a, const VeChar8* vvv)
//{
//	return 8;
//}
//
//VeInt32 TestFunc2(VeInt32 a, VeInt32 b, const VeChar8* vvv)
//{
//	return a + b;
//}
//
//void LuaTest()
//{
//	module(ve_lua, "main")
//	[
//		def("TestFunc", &TestFunc),
//		def("TestFunc", std::function<VeInt32(VeInt32)>(std::bind(&TestFunc, std::placeholders::_1, "abc"))),
//		def("TestFunc", &TestFunc2)
//	];
//
//	if (ve_lua.DoFile("test.lua"))
//	{
//		VeCoreLogE(VeLua::tostring(ve_lua, -1));
//		ve_lua.Pop();
//	}
//
//	call_function<void>("TestFunc");
//	VeInt32 r1 = call_function<VeInt32>("main.TestFunc", 5, "ttt");
//	VeInt32 r2 = call_function<VeInt32>("main.TestFunc", 5, 6, "ttt");
//	VeInt32 r3 = call_function<VeInt32>("main.TestFunc", 5, "ttt", 8);
//
//	r3 = call_function<VeInt32>("main.TestFunc", 5);
//
//	VeCoreLogD(r1, r2, r3);
//
//	//VeFloat32 res = call_function<VeFloat32>("TestAdd", 3.5, 4);
//
//	
//
//	//std::tuple<> ab = call_function<std::tuple<>>("TestAdd", 3.5, 4);
//
//
//	//std::tuple<VeFloat64, VeFloat64, VeFloat64> tu = call_function<std::tuple<VeFloat64, VeFloat64, VeFloat64>>("TestAdd", 3.5, 4);
//	std::tuple<VeFloat64, VeFloat64, VeFloat64> tu = call_func_mret<VeFloat64, VeFloat64, VeFloat64>(ve_lua, "TestAdd", 3.5, 4);
//
//	call_function("print", tu, "haha");
//	//std::tuple<VeFloat64> tu = std::make_tuple(0.1);
//
//	//std::tuple<int, float, const VeChar8*> tup(0, 5.5f, "abc");
//	//auto sp = venus::tuple_split<int, float, const VeChar8*>::split(tup);
//	//int a = 0;
//
//	//std::function<VeInt32(VeInt32, const VeChar8*)> func = TestFunc;
//
//	//VeInt32 a1 = sizeof(func);
//
//	//VeLua::pushinteger(ve_lua, 5);
//	//VeLua::pushstring(ve_lua, "test");
//
//	////VeInt32 res = venus::caller<1, VeInt32, std::function<VeInt32(VeInt32, const VeChar8*)>,venus::param_paker<VeInt32, const VeChar8*> >::call(
//	////	ve_lua, func);
//
//	//VeInt32 res = invoker<1, VeInt32, VeInt32, const VeChar8*>::invoke(ve_lua, func);
//
//	//VeInt32 ttt = param_getter<VeInt32>::get(ve_lua, -1);
//
//	//VeDebugOutput("%d", ttt);
//
//	//VeInt32 res = venus::caller<VeInt32, std::function<VeInt32(VeInt32, const VeChar8*)>,
//	//	venus::param_paker<VeInt32, const VeChar8*>>::call(func);
//
//}
