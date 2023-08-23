/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkConvolutionImageFilterBase.h"

namespace itk
{
/** Define how to print enumerations */
std::ostream &
operator<<(std::ostream & out, const ConvolutionImageFilterBaseEnums::ConvolutionImageFilterOutputRegion value)
{
  return out << [value] {
    switch (value)
    {
      case ConvolutionImageFilterBaseEnums::ConvolutionImageFilterOutputRegion::SAME:
        return "ConvolutionImageFilterBaseEnums::ConvolutionImageFilterOutputRegion::SAME";
      case ConvolutionImageFilterBaseEnums::ConvolutionImageFilterOutputRegion::VALID:
        return "ConvolutionImageFilterBaseEnums::ConvolutionImageFilterOutputRegion::VALID";
      default:
        return "INVALID VALUE FOR ConvolutionImageFilterBaseEnums::ConvolutionImageFilterOutputRegion";
    }
  }();
}
} // namespace itk
