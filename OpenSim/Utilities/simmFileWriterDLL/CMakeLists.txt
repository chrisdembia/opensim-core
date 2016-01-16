
file(GLOB SOURCES *.cpp)
file(GLOB INCLUDES *.h)

OpenSimAddLibrary(
    KIT SimmFileWriter
    AUTHORS "Peter_Loan"
    LINKLIBS osimSingle
    INCLUDES ${INCLUDES}
    SOURCES ${SOURCES}
    TESTDIRS ""
    )
