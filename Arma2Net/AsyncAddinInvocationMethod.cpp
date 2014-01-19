/*
* Copyright 2013 Arma2NET Developers
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "Addin.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

namespace Arma2Net
{
	public ref class AsyncAddinInvocationMethod : public IAddinInvocationMethod
	{
	private:
		initonly Addin^ addin;
		initonly Queue<String^>^ results = gcnew Queue<String^>;

		void InvokeImpl(Object^ obj)
		{
			auto tuple = (Tuple<String^, int>^)obj;
			auto result = addin->Invoke(tuple->Item1, tuple->Item2);
			results->Enqueue(result);
		}
	public:
		AsyncAddinInvocationMethod(Addin^ addin)
		{
			this->addin = addin;
		}

		virtual String^ Invoke(String^ args, int maxResultSize)
		{
			if (args == "getresult" || args->ToLower() == "getresult")
			{
				if (results->Count > 0)
					return results->Dequeue();
				return nullptr;
			}
			auto tuple = Tuple::Create(args, maxResultSize);
			Task::Factory->StartNew(gcnew Action<Object^>(this, &AsyncAddinInvocationMethod::InvokeImpl), tuple);
			return nullptr;
		}
	};
}