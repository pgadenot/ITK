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

#include "itkAffineGeometryFrame.h"
#include "itkSpatialObjectTreeContainer.h"
#include "itkInOrderTreeIterator.h"
#include "itkLevelOrderTreeIterator.h"
#include <iostream>

/**
 * This is a test file for the itkAffineGeometryFrame class.
 */
int itkAffineGeometryFrameTest(int, char* [])
{
  typedef itk::AffineGeometryFrame<> AffineGeometryFrameType;

  AffineGeometryFrameType::Pointer geometryFrame1 = AffineGeometryFrameType::New();
  std::cout << "Testing Print before initialization" << std::endl;
  geometryFrame1->Print(std::cout);

  std::cout << "Testing Initialize(): ";
  geometryFrame1->Initialize();
  std::cout<<"[DONE]"<<std::endl;

  std::cout << "Testing GetBoundingBox(): ";
  if(geometryFrame1->GetBoundingBox()==NULL)
    {
    std::cerr << "not initialized [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetIndexToObjectTransform(): ";
  if(geometryFrame1->GetIndexToObjectTransform()==NULL)
    {
    std::cerr << "not initialized [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetObjectToNodeTransform(): ";
  if(geometryFrame1->GetObjectToNodeTransform()==NULL)
    {
    std::cerr << "not initialized [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing SetBounds(): ";
  AffineGeometryFrameType::BoundingBoxType::CoordRepType bounds[6] = {3, 10, 5, 12, 7, 17};
  geometryFrame1->SetBounds(bounds);
  std::cout<<"[DONE]"<<std::endl;

  std::cout << "Testing GetExtent() of geometry-frame: ";
  if((geometryFrame1->GetExtent(0)!=bounds[1]-bounds[0]) ||
     (geometryFrame1->GetExtent(1)!=bounds[3]-bounds[2]) ||
     (geometryFrame1->GetExtent(2)!=bounds[5]-bounds[4]) )
    {
    std::cerr << " [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetIndexToObjectTransform()->SetOffset()/...->GetOffset(): ";
  AffineGeometryFrameType::TransformType::OffsetType offset;
  offset.Fill(10);
  geometryFrame1->GetIndexToObjectTransform()->SetOffset(offset);
  AffineGeometryFrameType::TransformType::OffsetType diff;
  diff = geometryFrame1->GetIndexToObjectTransform()->GetOffset()-offset;
  if(diff.GetNorm()!=0)
    {
    std::cerr << " [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing Clone(): ";
  AffineGeometryFrameType::Pointer clonedGeometryFrame1 = geometryFrame1->Clone();
  std::cout<<"[DONE]"<<std::endl;

  std::cout << "Testing GetBoundingBox() of cloned geometry-frame: ";
  if(clonedGeometryFrame1->GetBoundingBox()==NULL)
    {
    std::cerr << "not initialized [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetIndexToObjectTransform() of cloned geometry-frame: ";
  if(clonedGeometryFrame1->GetIndexToObjectTransform()==NULL)
    {
    std::cerr << "not initialized [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetObjectToNodeTransform() of cloned geometry-frame: ";
  if(clonedGeometryFrame1->GetObjectToNodeTransform()==NULL)
    {
    std::cerr << "not initialized [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetBoundingBox() of cloned geometry-frame: ";
  const AffineGeometryFrameType::BoundsArrayType& boundsArray = clonedGeometryFrame1->GetBoundingBox()->GetBounds();
  unsigned int i;
  for(i=0; i<6; ++i)
    if(boundsArray[i]!=bounds[i])
    {
    std::cerr << "clonedBounds[" <<i<< "]=" << boundsArray[i] << "!=" <<bounds[i] << "[FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetExtent() of cloned geometry-frame: ";
  if((clonedGeometryFrame1->GetExtent(0)!=bounds[1]-bounds[0]) ||
     (clonedGeometryFrame1->GetExtent(1)!=bounds[3]-bounds[2]) ||
     (clonedGeometryFrame1->GetExtent(2)!=bounds[5]-bounds[4]) )
    {
    std::cerr << " [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetIndexToObjectTransform()->GetOffset()  of cloned geometry-frame: ";
  diff = clonedGeometryFrame1->GetIndexToObjectTransform()->GetOffset()-offset;
  if(diff.GetNorm()!=0)
    {
    std::cerr << " [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing independency of IndexToObjectTransform of original and cloned geometry-frame: ";
  offset.Fill(15);
  geometryFrame1->GetIndexToObjectTransform()->SetOffset(offset);
  diff = clonedGeometryFrame1->GetIndexToObjectTransform()->GetOffset()-offset;
  if(diff.GetNorm()==0)
    {
    std::cerr << " [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing independency of ObjectToNodeTransform of original and cloned geometry-frame: ";
  geometryFrame1->GetObjectToNodeTransform()->SetOffset(offset);
  diff = clonedGeometryFrame1->GetObjectToNodeTransform()->GetOffset()-offset;
  if(diff.GetNorm()==0)
    {
    std::cerr << " [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "Testing Print after initialization" << std::endl;
  geometryFrame1->Print(std::cout);

  std::cout<<"[PASSED]"<<std::endl;

  std::cout<<"[TEST DONE]"<<std::endl;
  return EXIT_SUCCESS;
}

