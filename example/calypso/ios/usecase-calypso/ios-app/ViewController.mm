//
//  ViewController.m
//  ios-app
//
//  Created by Alexandre MUNSCH on 25/01/2021.
//  Copyright © 2021 Alexandre MUNSCH. All rights reserved.
//

#import "ViewController.h"

#include "UseCase_Calypso1_ExplicitSelectionAid_IOS.h"
#include "UseCase_Calypso2_DefaultSelectionNotification_IOS.h"

@interface ViewController ()

@property std::shared_ptr<UseCase_Calypso1_ExplicitSelectionAid_IOS> useCase_Calypso1_IOS;
@property std::shared_ptr<UseCase_Calypso2_DefaultSelectionNotification_IOS> useCase_Calypso2_IOS;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view, typically from a nib.
    self.useCase_Calypso1_IOS = std::make_shared<UseCase_Calypso1_ExplicitSelectionAid_IOS>(self);
    self.useCase_Calypso2_IOS = std::make_shared<UseCase_Calypso2_DefaultSelectionNotification_IOS>(self);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)onTouchUpInside:(id)sender {
    NSLog(@"ViewController - onTouchUpInside");

    if ([sender tag] == 1)
        self.useCase_Calypso1_IOS->start();
    else if ([sender tag] == 2)
        self.useCase_Calypso2_IOS->start();
}

- (void) sucess {

}

@end
