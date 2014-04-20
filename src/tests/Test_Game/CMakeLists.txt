#
# Copyright (c) 2013-2014 the Demi3D project.
#

DI_ADD_TEST(Test_Game)

# Copy hon resources
if (APPLE AND DEMI_BUILD_PLATFORM_APPLE_IOS)
#if (FALSE)
    add_custom_command(TARGET Test_Game POST_BUILD
      COMMAND ditto ${DEMI_SOURCE_DIR}/media_ios ${DEMI_SAMPLE_CONTENTS_PATH}/media_ios
    )
endif()