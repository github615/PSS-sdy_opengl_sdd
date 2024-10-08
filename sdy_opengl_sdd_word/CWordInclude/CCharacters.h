// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

//lrg #import "C:\\Program Files (x86)\\Microsoft Office\\Office12\\MSWORD.OLB" no_namespace
// CCharacters 包装类

class CCharacters : public COleDispatchDriver
{
public:
  CCharacters(){} // 调用 COleDispatchDriver 默认构造函数
  CCharacters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  CCharacters(const CCharacters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // 特性
public:

  // 操作
public:


  // Characters 方法
public:
  LPUNKNOWN get__NewEnum()
  {
    LPUNKNOWN result;
    InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
    return result;
  }
  long get_Count()
  {
    long result;
    InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  LPDISPATCH get_First()
  {
    LPDISPATCH result;
    InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
    return result;
  }
  LPDISPATCH get_Last()
  {
    LPDISPATCH result;
    InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
    return result;
  }
  LPDISPATCH get_Application()
  {
    LPDISPATCH result;
    InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
    return result;
  }
  long get_Creator()
  {
    long result;
    InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  LPDISPATCH get_Parent()
  {
    LPDISPATCH result;
    InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
    return result;
  }
  LPDISPATCH Item(long Index)
  {
    LPDISPATCH result;
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Index);
    return result;
  }

  // Characters 属性
public:

};
