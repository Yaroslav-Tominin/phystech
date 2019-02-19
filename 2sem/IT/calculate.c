#include "calculate.h"
float ReturnJson(JsonElement json1);

float FunctionSum(JsonElement json1,JsonElement json2)
{
    JsonElementValueType one=json1.type;
    JsonElementValueType two=json2.type;
    float json_ret1;
    float json_ret2;
    if(one==JSON_MAP)
    {
		/*json value*/
        json_ret1 =ReturnJson(json1);
    }
    else
    {
		json_ret1=atof(json1.value);
	}
	if(two==JSON_MAP)
    {
        json_ret2 =ReturnJson(json2);
    }
    else
    {
		json_ret2=atof(json2.value);
	}
	printf("Summ:arg1-%f,arg2-%f\n",json_ret1,json_ret2);
	return json_ret1+json_ret2;
}
float FunctionSub(JsonElement json1,JsonElement json2)
{
    JsonElementValueType one=json1.type;
    JsonElementValueType two=json2.type;
    float json_ret1;
    float json_ret2;
    if(one==JSON_MAP)
    {
		/*json value*/
        json_ret1 =ReturnJson(json1);
    }
    else
    {
		json_ret1=atof(json1.value);
	}
	if(two==JSON_MAP)
    {
        json_ret2 =ReturnJson(json2);
    }
    else
    {
		json_ret2=atof(json2.value);
	}
	printf("Sub:arg1-%f,arg2-%f\n",json_ret1,json_ret2);
	return json_ret1-json_ret2;
}

float FunctionMul(JsonElement json1,JsonElement json2)
{
    JsonElementValueType one=json1.type;
    JsonElementValueType two=json2.type;
    float json_ret1;
    float json_ret2;
    if(one==JSON_MAP)
    {
		/*json value*/
        json_ret1 =ReturnJson(json1);
    }
    else
    {
		json_ret1=atof(json1.value);
	}
	if(two==JSON_MAP)
    {
        json_ret2 =ReturnJson(json2);
    }
    else
    {
		json_ret2=atof(json2.value);
	}
	printf("Mul:arg1-%f,arg2-%f\n",json_ret1,json_ret2);
	return json_ret1*json_ret2;
}
float FunctionDiv(JsonElement json1,JsonElement json2)
{
    JsonElementValueType one=json1.type;
    JsonElementValueType two=json2.type;
    float json_ret1;
    float json_ret2;
    if(one==JSON_MAP)
    {
		/*json value*/
        json_ret1 =ReturnJson(json1);
    }
    else
    {
		json_ret1=atof(json1.value);
	}
	if(two==JSON_MAP)
    {
        json_ret2 =ReturnJson(json2);
    }
    else
    {
		json_ret2=atof(json2.value);
	}
	printf("Div:arg1-%f,arg2-%f\n",json_ret1,json_ret2);
	return json_ret1/json_ret2;
}
float ReturnJson(JsonElement json1)
{
	pHash h=(pHash)json1.value;
    if(IsInMap(h,"sum"))
    {
		/*mapa json include 2 arg*/
		pHash map=((pJsonElement)GetFromMap(h,"sum"))->value;
		float json_ret1 =FunctionSum(*((pJsonElement)GetFromMap(map,"arg1")),*((pJsonElement)GetFromMap(map,"arg2")));
		return json_ret1;
	}
	else if(IsInMap(h,"sub"))
    {
		/*mapa json include 2 arg*/
		pHash map=((pJsonElement)GetFromMap(h,"sub"))->value;
		float json_ret1 =FunctionSub(*((pJsonElement)GetFromMap(map,"arg1")),*((pJsonElement)GetFromMap(map,"arg2")));
		return json_ret1;
	}
	else if(IsInMap(h,"div"))
    {
		/*mapa json include 2 arg*/
		pHash map=((pJsonElement)GetFromMap(h,"div"))->value;
		float json_ret1 =FunctionDiv(*((pJsonElement)GetFromMap(map,"arg1")),*((pJsonElement)GetFromMap(map,"arg2")));
		return json_ret1;
	}
	else if(IsInMap(h,"mul"))
    {
		/*mapa json include 2 arg*/
		pHash map=((pJsonElement)GetFromMap(h,"mul"))->value;
		float json_ret1 =FunctionMul(*((pJsonElement)GetFromMap(map,"arg1")),*((pJsonElement)GetFromMap(map,"arg2")));
		return json_ret1;
	}
}

float ParsingCalculate(pHash h)
{
	float f=ReturnJson(*((pJsonElement)GetFromMap(h,"calculate")));
	return f;
}
