#ifndef AUXPORT_PARAM_H
#define AUXPORT_PARAM_H


/*
*			AuxParam : Extension for JUCE
			"I tried to make it easier" - inpinseptipin
			BSD 3-Clause License
			Copyright (c) 2023, Satyarth Arora
			All rights reserved.
			Redistribution and use in source and binary forms, with or without
			modification, are permitted provided that the following conditions are met:
			1. Redistributions of source code must retain the above copyright notice, this
			   list of conditions and the following disclaimer.
			2. Redistributions in binary form must reproduce the above copyright notice,
			   this list of conditions and the following disclaimer in the documentation
			   and/or other materials provided with the distribution.
			3. Neither the name of the copyright holder nor the names of its
			   contributors may be used to endorse or promote products derived from
			   this software without specific prior written permission.
			THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
			AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
			IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
			DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
			FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
			DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
			SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
			CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
			OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
			OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "JuceHeader.h"
namespace AuxPort
{
	
	class ParameterMap
	{
	public:
		ParameterMap(juce::AudioProcessor* audioProcessor)
		{
			this->audioProcessor = audioProcessor;
		}
		~ParameterMap() = default;
		ParameterMap(const ParameterMap& params) = default;
		enum type
		{
			aBool,aFloat,aInt
		};
/***********************************************************************************/
/*
	[Function] Use this to add a juce::AudioParameterBool pointer to the AuxParam Map
*/
/***********************************************************************************/
		void addParameter(juce::AudioParameterBool* boolParam)
		{
			for (uint32_t i = 0; i < parameterMap.size(); i++)
			{
				jassert(parameterMap[i].getParameterName() == boolParam->name);
				DBG("Try using a Different Parameter Name");
			}
			boolParameters.push_back(boolParam);
			parameterMap.push_back({ boolParameters.size() - 1,AuxPort::ParameterMap::type::aBool,boolParam->name });
		}
/***********************************************************************************/
/*
	[Function] Use this to add a juce::AudioParameterFloat pointer to the AuxParam Map
*/
/***********************************************************************************/
		void addParameter(juce::AudioParameterFloat* floatParam)
		{
			for (uint32_t i = 0; i < parameterMap.size(); i++)
			{
				jassert(parameterMap[i].getParameterName() != floatParam->name);
				DBG("Try using a Different Parameter Name");
			}
			floatParameters.push_back(floatParam);
			parameterMap.push_back({ floatParameters.size() - 1,AuxPort::ParameterMap::type::aFloat,floatParam->name });
		}

/***********************************************************************************/
/*
	[Function] Use this to add a juce::AudioParameterInt pointer to the AuxParam Map
*/
/***********************************************************************************/

		void addParameter(juce::AudioParameterInt* intParam)
		{
			for (uint32_t i = 0; i < parameterMap.size(); i++)
			{
				jassert(parameterMap[i].getParameterName() != intParam->name);
				DBG("Try using a Different Parameter Name");
			}
			intParameters.push_back(intParam);
			parameterMap.push_back({ intParameters.size() - 1,AuxPort::ParameterMap::type::aInt,intParam->name });
		}

/***********************************************************************************/
/*
	[Function] Pass the parameter name to this function, and get the juce::AudioParameterFloat pointer.
*/
/***********************************************************************************/
		juce::AudioParameterFloat* getFloatParameter(const juce::String& parameterName)
		{
			for (uint32_t i = 0; i < parameterMap.size(); i++)
			{
				AuxPort::ParameterMap::parameter* param = &parameterMap[i];
				if (param->getParameterType() == AuxPort::ParameterMap::aFloat)
					if (param->getParameterName() == parameterName)
						return floatParameters[param->getParameterPosition()];
			}
			jassert(true);
			DBG("There is no AudioProcessorFloat parameter with that name");
		}
/***********************************************************************************/
/*
	[Function] Pass the parameter name to this function, and get the juce::AudioParameterBool pointer.
*/
/***********************************************************************************/

		juce::AudioParameterBool* getBoolParameter(const juce::String& parameterName)
		{
			for (uint32_t i = 0; i < parameterMap.size(); i++)
			{
				AuxPort::ParameterMap::parameter* param = &parameterMap[i];
				if (param->getParameterType() == AuxPort::ParameterMap::aBool)
					if (param->getParameterName() == parameterName)
						return boolParameters[param->getParameterPosition()];
			}
			jassert(true);
			DBG("There is no AudioProcessorBool parameter with that name");
		}

/***********************************************************************************/
/*
	[Function] Pass the parameter name to this function, and get the juce::AudioParameterInt pointer.
*/
/***********************************************************************************/
		juce::AudioParameterInt* getIntParameter(const juce::String& parameterName)
		{
			for (uint32_t i = 0; i < parameterMap.size(); i++)
			{
				AuxPort::ParameterMap::parameter* param = &parameterMap[i];
				if (param->getParameterType() == AuxPort::ParameterMap::aInt)
					if (param->getParameterName() == parameterName)
						return intParameters[param->getParameterPosition()];
			}
			jassert(true);
			DBG("There is no AudioProcessorInt parameter with that Name");
		}
/***********************************************************************************/	
/*
	[Function] Call this function to save your parameters so that they can be seen by the DAW
*/
/***********************************************************************************/

		void save()
		{
			for (uint32_t i = 0; i < parameterMap.size(); i++)
			{
				AuxPort::ParameterMap::parameter* param = &parameterMap[i];
				if (param->getParameterType() == AuxPort::ParameterMap::aBool)
					audioProcessor->addParameter(boolParameters[param->getParameterPosition()]);
				if (param->getParameterType() == AuxPort::ParameterMap::aFloat)
					audioProcessor->addParameter(floatParameters[param->getParameterPosition()]);
				if (param->getParameterType() == AuxPort::ParameterMap::aInt)
					audioProcessor->addParameter(intParameters[param->getParameterPosition()]);
			}
		}
		
		
	private:
		class parameter
		{
		public:
			parameter() = default;
			~parameter() = default;
			parameter(const parameter& param) = default;
			parameter(const size_t& index, const AuxPort::ParameterMap::type& paramType,const juce::String& paramName)
			{
				p_pos = index;
				p_type = paramType;
				p_name = paramName;
			}
			juce::String getParameterName()
			{
				return p_name;
			}
			uint32_t getParameterPosition()
			{
				return p_pos;
			}
			AuxPort::ParameterMap::type getParameterType()
			{
				return p_type;
			}
		private:
			uint32_t p_pos;
			AuxPort::ParameterMap::type p_type;
			juce::String p_name;
		};
		juce::AudioProcessor* audioProcessor;
		std::vector<juce::AudioParameterBool*> boolParameters;
		std::vector<juce::AudioParameterFloat*> floatParameters;
		std::vector<juce::AudioParameterInt*> intParameters;
		std::vector<AuxPort::ParameterMap::parameter> parameterMap;
		
		
	};
}
#endif


