/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#include "MiscPch.h"
#include "PathLib.h"

// iOS platform doesn't need this sort of tools
#if DEMI_PLATFORM == DEMI_PLATFORM_OSX

#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>
#import <Cocoa/Cocoa.h>

    class FCocoaScopeContext
    {
    public:
        FCocoaScopeContext(void)
        {
            NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
            PreviousContext = [NSOpenGLContext currentContext];
            [pool drain];
        }
        
        ~FCocoaScopeContext( void )
        {
            NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
            NSOpenGLContext* NewContext = [NSOpenGLContext currentContext];
            if (PreviousContext != NewContext)
            {
                if (PreviousContext)
                    [PreviousContext makeCurrentContext];
                else
                    [NSOpenGLContext clearCurrentContext];
            }
            [pool drain];
        }
        
    private:
        NSOpenGLContext*	PreviousContext;
    };

    @interface FFileDialogAccessoryView : NSView
    {
    @private
    	NSPopUpButton*	PopUpButton;
    	NSTextField*	TextField;
    	NSSavePanel*	DialogPanel;
    	NSMutableArray*	AllowedFileTypes;
    }

    - (id)initWithFrame:(NSRect)frameRect dialogPanel:(NSSavePanel*) panel;
    - (void)PopUpButtonAction: (id) sender;
    - (void)AddAllowedFileTypes: (NSArray*) array;
    - (void)SetExtensionsAtIndex: (ulong) index;

    @end
        
    @implementation FFileDialogAccessoryView

    - (id)initWithFrame:(NSRect)frameRect dialogPanel:(NSSavePanel*) panel
    {
    	self = [super initWithFrame: frameRect];
    	DialogPanel = panel;
    	
    	CFStringRef FieldTextCFString = CFSTR("File Extension:");
    	TextField = [[NSTextField alloc] initWithFrame: NSMakeRect(0.0, 48.0, 90.0, 25.0) ];
    	[TextField setStringValue:(NSString*)FieldTextCFString];
    	[TextField setEditable:NO];
    	[TextField setBordered:NO];
    	[TextField setBackgroundColor:[NSColor controlColor]];
    	
        
    	PopUpButton = [[NSPopUpButton alloc] initWithFrame: NSMakeRect(88.0, 50.0, 160.0, 25.0) ];
    	[PopUpButton setTarget: self];
    	[PopUpButton setAction:@selector(PopUpButtonAction:)];
        
    	[self addSubview: TextField];
    	[self addSubview: PopUpButton];
        
    	return self;
    }

    - (void)AddAllowedFileTypes: (NSMutableArray*) array
    {
    	check( array );
        
    	AllowedFileTypes = array;
    	int32 ArrayCount = [AllowedFileTypes count];
    	if( ArrayCount )
    	{
    		check( ArrayCount % 2 == 0 );
            
    		[PopUpButton removeAllItems];
            
    		for( int32 Index = 0; Index < ArrayCount; Index += 2 )
    		{
    			[PopUpButton addItemWithTitle: [AllowedFileTypes objectAtIndex: Index]];
    		}
            
    		[self SetExtensionsAtIndex: 0];
    	}
    	else
    	{
    		[DialogPanel setAllowedFileTypes:nil];
    	}
    }

    - (void)PopUpButtonAction: (id) sender
    {
    	NSInteger Index = [PopUpButton indexOfSelectedItem];
    	[self SetExtensionsAtIndex: Index];
    }

    - (void)SetExtensionsAtIndex: (ulong) index
    {
    	check( [AllowedFileTypes count] >= index * 2 );
        
    	NSString* ExtsToParse = [AllowedFileTypes objectAtIndex:index * 2 + 1];
    	if( [ExtsToParse compare:@"*.*"] == NSOrderedSame )
    	{
    		[DialogPanel setAllowedFileTypes: nil];
    	}
    	else
    	{
    		NSArray* ExtensionsWildcards = [ExtsToParse componentsSeparatedByString:@";"];
    		NSMutableArray* Extensions = [NSMutableArray arrayWithCapacity: [ExtensionsWildcards count]];
            
    		for( ulong Index = 0; Index < [ExtensionsWildcards count]; ++Index )
    		{
    			NSString* Temp = [[ExtensionsWildcards objectAtIndex:Index] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"*."]];
    			[Extensions addObject: Temp];
    		}
            
    		[DialogPanel setAllowedFileTypes: Extensions];
    	}
    }

    @end

    @interface FFontDialogAccessoryView : NSView
    {
    @private
        
    	NSButton*	OKButton;
    	NSButton*	CancelButton;
    	bool		Result;
    }

    - (id)initWithFrame: (NSRect)frameRect;
    - (bool)result;
    - (IBAction)onCancel: (id)sender;
    - (IBAction)onOK: (id)sender;

    @end
    
    @implementation FFontDialogAccessoryView : NSView

    - (id)initWithFrame: (NSRect)frameRect
    {
    	[super initWithFrame: frameRect];
        
    	CancelButton = [[NSButton alloc] initWithFrame: NSMakeRect(10, 10, 80, 24)];
    	[CancelButton setTitle: @"Cancel"];
    	[CancelButton setBezelStyle: NSRoundedBezelStyle];
    	[CancelButton setButtonType: NSMomentaryPushInButton];
    	[CancelButton setAction: @selector(onCancel:)];
    	[CancelButton setTarget: self];
    	[self addSubview: CancelButton];
        
    	OKButton = [[NSButton alloc] initWithFrame: NSMakeRect(100, 10, 80, 24)];
    	[OKButton setTitle: @"OK"];
    	[OKButton setBezelStyle: NSRoundedBezelStyle];
    	[OKButton setButtonType: NSMomentaryPushInButton];
    	[OKButton setAction: @selector(onOK:)];
    	[OKButton setTarget: self];
    	[self addSubview: OKButton];
        
    	Result = false;
        
    	return self;
    }

    - (bool)result
    {
    	return Result;
    }

    - (IBAction)onCancel: (id)sender
    {
    	Result = false;
    	[NSApp stopModal];
    }

    - (IBAction)onOK: (id)sender
    {
    	Result = true;
    	[NSApp stopModal];
    }

    @end

namespace Demi
{
    static bool FileDialogShared(bool bSave, const void* wndHandle, const DiString& dlgTitle, const DiString& defaultPath, const DiString& defaultFile, const DiString& fileTypes, uint32 flags, DiVector<DiString>& outFiles)
    {
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        FCocoaScopeContext ContextGuard;
        
        NSSavePanel* Panel = bSave ? [NSSavePanel savePanel] : [NSOpenPanel openPanel];
        
        if (!bSave)
        {
            NSOpenPanel* OpenPanel = (NSOpenPanel*)Panel;
            [OpenPanel setCanChooseFiles: true];
            [OpenPanel setCanChooseDirectories: false];
            [OpenPanel setAllowsMultipleSelection: flags & DiPathLib::MULTIPLE_SELECTION];
        }
        
        [Panel setCanCreateDirectories: bSave];
        
        [Panel setTitle: [NSString stringWithCString:dlgTitle.c_str() encoding:NSUTF8StringEncoding]];
        
        NSURL* DefaultPathURL = [NSURL fileURLWithPath: [NSString stringWithCString:defaultPath.c_str() encoding:NSUTF8StringEncoding]];
        [Panel setDirectoryURL: DefaultPathURL];
        [Panel setNameFieldStringValue: [NSString stringWithCString:defaultFile.c_str() encoding:NSUTF8StringEncoding]];
        
        FFileDialogAccessoryView* AccessoryView = [[FFileDialogAccessoryView alloc] initWithFrame: NSMakeRect( 0.0, 0.0, 250.0, 85.0 ) dialogPanel: Panel];
        [Panel setAccessoryView: AccessoryView];
        
        DiVector<DiString> FileTypesArray = fileTypes.Tokenize("|");
        int32 NumFileTypes = (int32)FileTypesArray.size();
        
        NSMutableArray* AllowedFileTypes = [NSMutableArray arrayWithCapacity: NumFileTypes];
        
        if( NumFileTypes > 0 )
        {
            for( int32 Index = 0; Index < NumFileTypes; ++Index )
            {
                [AllowedFileTypes addObject: [NSString stringWithCString:FileTypesArray[Index].c_str() encoding:NSUTF8StringEncoding]];
            }
        }
        
        [AccessoryView AddAllowedFileTypes:AllowedFileTypes];
        
        bool bSuccess = false;
        
        {
            NSInteger Result = [Panel runModal];
            [AccessoryView release];
            
            if (Result == NSFileHandlingPanelOKButton)
            {
                if (bSave)
                {
                    outFiles.resize(1);
                    outFiles[0] = [[[Panel URL] path] UTF8String];
                }
                else
                {
                    NSOpenPanel* OpenPanel = (NSOpenPanel*)Panel;
                    outFiles.clear();
                    for (NSURL *FileURL in [OpenPanel URLs])
                    {
                        outFiles.push_back([[FileURL path] UTF8String]);
                    }
                }
                
                bSuccess = true;
            }
        }
        
        [Panel close];
        
        [pool drain];
        return bSuccess;
    }
    
    bool DiPathLib::OpenFileDialog(const void* wndHandle, const DiString& title,
                                   const DiString& defaultPath, const DiString& defaultFile,
                                   const DiString& fileTypes, uint32 flags, DiVector<DiString>& outFiles)
    {
        return FileDialogShared(false, wndHandle, title, defaultPath, defaultFile, fileTypes, flags, outFiles);
    }
    
    bool DiPathLib::SaveFileDialog(const void* wndHandle, const DiString& title, const DiString& defaultPath,
                                   const DiString& defaultFile, const DiString& fileTypes,
                                   uint32 flags, DiVector<DiString>& outFiles)
    {
        return FileDialogShared(true, wndHandle, title, defaultPath, defaultFile, fileTypes, flags, outFiles);
    }
    
    bool DiPathLib::OpenDirectoryDialog(const void* wndHandle, const DiString& dlgTitle, const DiString& defaultPath, DiString& outFolder)
    {
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        FCocoaScopeContext ContextGuard;
        
        NSOpenPanel* Panel = [NSOpenPanel openPanel];
        [Panel setAllowsMultipleSelection: false];
        [Panel setCanCreateDirectories: true];
        [Panel setCanChooseFiles: false];
        [Panel setCanChooseDirectories: true];
        
        [Panel setTitle: [NSString stringWithCString:dlgTitle.c_str() encoding:NSUTF8StringEncoding]];
        
        NSURL* DefaultPathURL = [NSURL fileURLWithPath: [NSString stringWithCString:defaultPath.c_str() encoding:NSUTF8StringEncoding]];
        [Panel setDirectoryURL: DefaultPathURL];
        
        bool bSuccess = false;
        
        NSInteger Result = [Panel runModal];
        
        if (Result == NSFileHandlingPanelOKButton)
        {
            NSURL *FolderURL = [[Panel URLs] objectAtIndex: 0];
            outFolder = [[FolderURL path] UTF8String];
            bSuccess = true;
        }
        
        [Panel close];
        
        [pool drain];
        return bSuccess;
    }
    
    void DiPathLib::ResetCurrentDir()
    {
    }
}

#endif