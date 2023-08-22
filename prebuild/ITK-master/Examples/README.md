ITK Software Guide Examples
---------------------------

This directory contains simple, companion examples to the ITK Software Guide.
These examples are designed to demonstrate features of the system; they are not
meant to be significant applications of the software.

For a more comprehensive set of self-contained, illustrative examples, see the
[ITKSphinxExamples](https://itk.org/ITKExamples).

Specifically, the purpose of these examples is as follows:

 * Provide simple, minimalist examples of important features of ITK and how
   to use them. The examples have minimal dependencies on outside packages;
   and if they do, there should be CMake flags to turn them off.

 * Provide a consistent set of examples that will work in conjunctions with
   the ITK Software Guide. The ITK Software Guide sources are found in the
   separate [ITKSoftwareGuide](https://github.com/InsightSoftwareConsortium/ITKSoftwareGuide) repository.

 * Make sure that the code is well documented, of consistent style, and
   always up-to-date with the current ITK code.

The following is a list of subdirectories with a description of the code
found in them:

 * **Installation** - a very simple example to demonstrate compiling against
   the ITK libraries and configuring [CMake](https://cmake.org/).

 * **Data representation** - creating images and meshes; shows the basics of
   creating and executing the pipeline

 * **Iterators** - iterating over images and meshes.

 * **Infrastructure** - events, observers, factories, smart pointers,
   namespaces, transforms, etc.

 * **Numerics** - working with [VNL](https://vxl.github.io/); a focus on
   interface with ITK classes.

 * **IO** - the basics of reading/writing data.

 * **Filtering** - examples of image processing and mesh filters.

 * **Segmentation** - a suite of basic segmentation examples.

 * **Registration** - a suite of basic registration methods.

To learn the software from these examples, you may wish to refer to the
[ITK Software Guide](https://itk.org/ItkSoftwareGuide.pdf), whose source code can be found at

  https://github.com/InsightSoftwareConsortium/ITKSoftwareGuide
