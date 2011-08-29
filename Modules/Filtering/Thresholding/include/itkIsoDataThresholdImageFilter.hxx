#ifndef __itkIsoDataThresholdImageFilter_hxx
#define __itkIsoDataThresholdImageFilter_hxx
#include "itkIsoDataThresholdImageFilter.h"

#include "itkImageToHistogramFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkIsoDataThresholdImageCalculator.h"
#include "itkProgressAccumulator.h"

namespace itk {

template<class TInputImage, class TOutputImage>
IsoDataThresholdImageFilter<TInputImage, TOutputImage>
::IsoDataThresholdImageFilter()
{
  m_OutsideValue   = NumericTraits<OutputPixelType>::Zero;
  m_InsideValue    = NumericTraits<OutputPixelType>::max();
  m_Threshold      = NumericTraits<InputPixelType>::Zero;
  m_NumberOfHistogramBins = 128;
}

template<class TInputImage, class TOutputImage>
void
IsoDataThresholdImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  typename ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  typedef itk::Statistics::ImageToHistogramFilter<InputImageType> HistogramGeneratorType;
  typedef typename HistogramGeneratorType::HistogramType          HistogramType;
  typename HistogramGeneratorType::Pointer histogramGenerator = HistogramGeneratorType::New();
  histogramGenerator->SetInput( this->GetInput() );
  histogramGenerator->SetNumberOfBins( this->GetNumberOfHistogramBins() );
  histogramGenerator->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(histogramGenerator,.4f);

  // Compute the IsoData Threshold for the input image
  typedef IsoDataThresholdImageCalculator<HistogramType> CalculatorType;
  typename CalculatorType::Pointer calculator = CalculatorType::New();
  calculator->SetInput( histogramGenerator->GetOutput() );
  calculator->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(calculator,.2f);

  typedef BinaryThresholdImageFilter<TInputImage,TOutputImage> ThresholderType;
  typename ThresholderType::Pointer thresholder = ThresholderType::New();
  threshold->SetInput (this->GetInput());
  thresholder->SetLowerThreshold(NumericTraits<InputPixelType>::NonpositiveMin());
  thresholder->SetUpperThresholdInput( calculator->GetOutput() );
  thresholder->SetInsideValue( this->GetInsideValue() );
  thresholder->SetOutsideValue( this->GetOutsideValue() );
  thresholder->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(threshold,.4f);

  thresholder->GraftOutput( this->GetOutput() );
  threshold->Update();
  this->GraftOutput( threshold->GetOutput() );
}

template<class TInputImage, class TOutputImage>
void
IsoDataThresholdImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  TInputImage * input = const_cast<TInputImage *>(this->GetInput());
  if( input )
    {
    input->SetRequestedRegionToLargestPossibleRegion();
    }
}

template<class TInputImage, class TOutputImage>
void
IsoDataThresholdImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "OutsideValue: "
     << static_cast<typename NumericTraits<OutputPixelType>::PrintType>(m_OutsideValue) << std::endl;
  os << indent << "InsideValue: "
     << static_cast<typename NumericTraits<OutputPixelType>::PrintType>(m_InsideValue) << std::endl;
  os << indent << "NumberOfHistogramBins: "
     << m_NumberOfHistogramBins << std::endl;
  os << indent << "Threshold (computed): "
     << static_cast<typename NumericTraits<InputPixelType>::PrintType>(m_Threshold) << std::endl;

}


}// end namespace itk
#endif
