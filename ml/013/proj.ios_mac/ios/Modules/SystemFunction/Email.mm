//
//  Email.cpp
//  Chups006
//
//  Created by QAMAC01 on 14-9-24.
//
//

#import "Email.h"
#import "AppController.h"

@implementation Email

- (void)sendEmailWithTitle:(const char* )title  withContent:(const char *)content withImage:(const char *)image withDelegate:(id)delegate;
{

    
    NSMutableString *emailBody = [[NSMutableString alloc] initWithString:@"<html><body>"];
    [emailBody appendString:[[NSString alloc] initWithUTF8String:content]] ;
    NSData *imageData = UIImagePNGRepresentation([[UIImage alloc] initWithContentsOfFile:[[NSString alloc] initWithUTF8String:image]]);
    [emailBody appendString:@"</body></html>"];
    Class mailClass = (NSClassFromString(@"MFMailComposeViewController"));
	if (nil != mailClass) {
        MFMailComposeViewController* emailDialog = [[MFMailComposeViewController alloc] init];
        if (nil != emailDialog) {
            emailDialog.mailComposeDelegate = delegate;
            // Set the subject of email
            [emailDialog addAttachmentData:imageData mimeType:@"image/png" fileName:@"ChupaChups"];
            [emailDialog setSubject:[[NSString alloc] initWithUTF8String:title]];
            [emailDialog setMessageBody:emailBody isHTML:YES];
            [[AppController sharedAppController].viewController presentModalViewController:emailDialog animated:YES];
            [emailDialog release];
        }
    }
    [emailBody release];


}

- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError *)error {
	NSLog(@"mail completed! good %i",result);
	NSString* msg= nil;
    
	if (result == MFMailComposeResultSaved) {
		msg = @"Email Saved!";
	}else if (result == MFMailComposeResultSent) {
		msg = @"Email Sent!";
	}else if (result == MFMailComposeResultFailed) {
		msg = @"Send Email Failed,Please Check Your Network Connection!";
	}else {
		[[AppController sharedAppController].viewController dismissModalViewControllerAnimated:YES];
		return;
	}
	[[AppController sharedAppController].viewController dismissModalViewControllerAnimated:YES];
	UIAlertView *alert = [[[UIAlertView  alloc] initWithTitle:nil
                           
                                                      message:msg
                           
                                                     delegate:nil
                           
                                            cancelButtonTitle:nil
                           
                                            otherButtonTitles:@"OK",nil] autorelease];
	[alert show];
}

@end

