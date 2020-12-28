#!groovy
@Library('keyple-ops') _
def keypleVersion
pipeline {
    agent {
        kubernetes {
            label 'keyple-cpp'
            yaml cppBuilder('7')
        }
    }
    stages {
        stage('Keyple Cpp: Build') {
            steps{
                container('java-builder') {
                    dir('build') {
                        sh 'cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain/gcc-linux.cmake ..'
                        sh 'make'
                    }
                }
            }
        }
    }
}
