/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkImageToHistogramFilter_h
#define __itkImageToHistogramFilter_h

#include "itkHistogram.h"
#include "itkImageTransformer.h"
#include "itkBarrier.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkProgressReporter.h"

namespace itk
{
namespace Statistics
{
/** \class ImageToHistogramFilter
 *  \brief This class generates an histogram from an image.
 *
 *  The concept of Histogram in ITK is quite generic. It has been designed to
 *  manage multiple components data. This class facilitates the computation of
 *  an histogram from an image. Internally it creates a List that is feed into
 *  the SampleToHistogramFilter.
 *
 * \ingroup ITKStatistics
 */

template< class TImage >
class ITK_EXPORT ImageToHistogramFilter:public ImageTransformer<TImage>
{
public:
  /** Standard typedefs */
  typedef ImageToHistogramFilter     Self;
  typedef ImageTransformer<TImage>   Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToHistogramFilter, ImageTransformer);

  /** standard New() method support */
  itkNewMacro(Self);

  typedef TImage                                         ImageType;
  typedef typename ImageType::PixelType                  PixelType;
  typedef typename ImageType::RegionType                 RegionType;
  typedef typename NumericTraits< PixelType >::ValueType ValueType;
  typedef typename NumericTraits< ValueType >::RealType  ValueRealType;

  typedef Histogram< ValueRealType >                    HistogramType;
  typedef typename HistogramType::Pointer               HistogramPointer;
  typedef typename HistogramType::ConstPointer          HistogramConstPointer;
  typedef typename HistogramType::SizeType              HistogramSizeType;
  typedef typename HistogramType::MeasurementType       HistogramMeasurementType;
  typedef typename HistogramType::MeasurementVectorType HistogramMeasurementVectorType;

public:

  /** Return the output histogram. */
  const HistogramType * GetOutput() const;
  HistogramType * GetOutput();

  /** Type of DataObjects to use for Size inputs */
  typedef SimpleDataObjectDecorator<
    HistogramSizeType > InputHistogramSizeObjectType;

  /** Type of DataObjects to use for Marginal Scale inputs */
  typedef SimpleDataObjectDecorator<
    HistogramMeasurementType > InputHistogramMeasurementObjectType;

  /** Type of DataObjects to use for Minimum and Maximums values of the
   * histogram bins. */
  typedef SimpleDataObjectDecorator<
    HistogramMeasurementVectorType > InputHistogramMeasurementVectorObjectType;

  /** Type of DataObjects to use for AutoMinimumMaximum input */
  typedef SimpleDataObjectDecorator< bool > InputBooleanObjectType;

  /** Methods for setting and getting the histogram size.  The histogram size
   * is encapsulated inside a decorator class. For this reason, it is possible
   * to set and get the decorator class, but it is only possible to set the
   * histogram size by value. This macro declares the methods
   * SetHistogramSize(), SetHistogramSizeInput(), GetHistogramSizeInput().
   */
  itkSetDecoratedInputMacro(HistogramSize, HistogramSizeType, 1);

  /** Methods for setting and getting the Marginal scale value.  The marginal
   * scale is used when the type of the measurement vector componets are of
   * integer type. */
  itkSetDecoratedInputMacro(MarginalScale, HistogramMeasurementType, 2);

  /** Methods for setting and getting the Minimum and Maximum values of the
   * histogram bins. */
  itkSetDecoratedInputMacro(HistogramBinMinimum, HistogramMeasurementVectorType, 3);
  itkSetDecoratedInputMacro(HistogramBinMaximum, HistogramMeasurementVectorType, 4);

  /** Methods for setting and getting the boolean flag that defines whether the
   * minimum and maximum of the histogram are going to be computed
   * automatically from the values of the sample */
  itkSetDecoratedInputMacro(AutoMinimumMaximum, bool, 5);

  /** Method that facilitates the use of this filter in the internal
   * pipeline of another filter. */
  virtual void GraftOutput(DataObject *output);

protected:
  ImageToHistogramFilter();
  virtual ~ImageToHistogramFilter() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

  void BeforeThreadedGenerateData(void);
  void ThreadedGenerateData(const RegionType & inputRegionForThread, ThreadIdType threadId);
  void AfterThreadedGenerateData(void);

  /** Method that construct the outputs */
  DataObject::Pointer  MakeOutput(unsigned int);

  virtual void ThreadedComputeMinimumAndMaximum( const RegionType & inputRegionForThread, ThreadIdType threadId, ProgressReporter & progress );
  virtual void ThreadedComputeHistogram( const RegionType & inputRegionForThread, ThreadIdType threadId, ProgressReporter & progress );

  std::vector< HistogramPointer >               m_Histograms;
  std::vector< HistogramMeasurementVectorType > m_Minimums;
  std::vector< HistogramMeasurementVectorType > m_Maximums;

private:
  ImageToHistogramFilter(const Self &); //purposely not implemented
  void operator=(const Self &);         //purposely not implemented

  void ApplyMarginalScale( HistogramMeasurementVectorType & min, HistogramMeasurementVectorType & max, HistogramSizeType & size );
  typename Barrier::Pointer                     m_Barrier;

};
} // end of namespace Statistics
} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToHistogramFilter.hxx"
#endif

#endif
