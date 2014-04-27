/**
 Provides FTP client.
 
 Consider implementing more of the commands specified at:
 http://en.wikipedia.org/wiki/List_of_FTP_commands
 
 */

#import "FTPHandle.h"
#import "FTPCredentials.h"

@class FTPClient;

@protocol FTPRequestDelegate;

@interface FTPClient : NSObject

/** Credentials used to login to the server. */
@property (nonatomic, readonly) FTPCredentials* credentials;

/**
 The last encountered error. Please note that this value does not get nil'ed
 when a new operation takes place. Therefore, do not use 'lastError' as a way
 to determine if the last operation succeeded or not. Check the return value
 first _then_ get the lastError.
 */
@property (nonatomic, readonly) NSError *lastError;

/**
 Factory method to create FTPClient instance.
 
 @param FTPLocation The location's credentials
 @return FTPClient
 */
+ (instancetype)clientWithCredentials:(FTPCredentials *)credentials;

/**
 Factory method to create FTPClient instance.
 
 @param host Server host to connect to
 @param port Server port.
 @param username Username to login as.
 @param password Password of user.
 @return FTPClient
 */
+ (instancetype)clientWithHost:(NSString *)host port:(int)port username:(NSString *)username password:(NSString* )password;

/**
 Create an instance of FTPClient.
 
 @param FTPLocation The location's credentials
 @return FTPClient
 */
- (instancetype)initWithCredentials:(FTPCredentials *)credentials;

/**
 Create an instance of FTPClient.
 
 @param host Server host to connect to.
 @param port Server port.
 @param username Username to login as.
 @param password Password of user.
 @return FTPClient
 */
- (instancetype)initWithHost:(NSString *)host port:(int)port username:(NSString *)username password:(NSString* )password;

/**
 Get the size, in bytes, for remote file at 'path'. This can not be used
 for directories.
 
 @param path Path to get size in bytes for.
 @return The size of the file in bytes. -1 if file doesn't exist.
 */
- (long long int)fileSizeAtPath:(NSString *)path;

/**
 List directory contents at path.
 
 @param path Path to remote directory to list.
 @param showHiddenItems Show hidden items in directory.
 @return List of contents as FTPHandle objects.
 */
- (NSArray *)listContentsAtPath:(NSString *)path showHiddenFiles:(BOOL)showHiddenFiles;

/**
 Refer to listContentsAtPath:showHiddenFiles:
 
 This adds the ability to perform the operation asynchronously.
 
 @param path Path to remote directory to list.
 @param showHiddenItems Show hidden items in directory.
 @param success Method called when process succeeds. Provides list of contents
        as FTPHandle objects.
 @param failure Method called when process fails.
 */
- (void)listContentsAtPath:(NSString *)path showHiddenFiles:(BOOL)showHiddenFiles
                   success:(void (^)(NSArray *contents))success
                   failure:(void (^)(NSError *error))failure;

/**
 List directory contents at handle's location.
 
 @param handle Remote directory handle to list.
 @param showHiddenItems Show hidden items in directory.
 @return List of contents as FTPHandle objects.
 */
- (NSArray *)listContentsAtHandle:(FTPHandle *)handle showHiddenFiles:(BOOL)showHiddenFiles;

/**
 Refer to listContentsAtHandle:showHiddenFiles:
 
 This adds the ability to perform the operation asynchronously.
 
 @param showHiddenItems Show hidden items in directory.
 @param success Method called when process succeeds. Provides list of contents
        as FTPHandle objects.
 @param failure Method called when process fails.
 */
- (void)listContentsAtHandle:(FTPHandle *)handle showHiddenFiles:(BOOL)showHiddenFiles
                     success:(void (^)(NSArray *contents))success
                     failure:(void (^)(NSError *error))failure;

/**
 Download remote file path to local path.
 
 @param fileName Full path of remote file to download.
 @param localPath Local path to download file to.
 @param progress Calls after data has been received to remote server.
        Return NO to cancel the operation.
 @return YES on success. NO on failure.
 */
- (BOOL)downloadFile:(NSString *)remotePath to:(NSString *)localPath
            progress:(BOOL (^)(NSUInteger received, NSUInteger totalBytes))progress;

/**
 Refer to downloadFile:to:progress:
 
 This adds the ability to perform the operation asynchronously.
 
 @param fileName Full path of remote file to download.
 @param localPath Local path to download file to.
 @param progress Calls after data has been received to remote server.
        Return NO to cancel the operation.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 */
- (void)downloadFile:(NSString *)remotePath to:(NSString *)localPath
            progress:(BOOL (^)(NSUInteger received, NSUInteger totalBytes))progress
             success:(void (^)(void))success
             failure:(void (^)(NSError *error))failure;

/**
 Download handle at specific location.
 
 @param handle Handle to download. Handles are produced by listDirectory* and friends.
 @param localPath Local path to download file to.
 @param progress Calls after data has been received to remote server.
        Return NO to cancel the operation.
 @return YES on success. NO on failure.
 */
- (BOOL)downloadHandle:(FTPHandle *)handle to:(NSString *)localPath
              progress:(BOOL (^)(NSUInteger received, NSUInteger totalBytes))progress;

/**
 Refer to downloadHandle:to:progress:
 
 This adds the ability to perform the operation asynchronously.
 
 @param handle Handle to download. Handles are produced by listDirectory* and friends.
 @param localPath Local path to download file to.
 @param progress Calls after data has been received to remote server.
        Return NO to cancel the operation.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 */
- (void)downloadHandle:(FTPHandle *)handle to:(NSString *)localPath
              progress:(BOOL (^)(NSUInteger received, NSUInteger totalBytes))progress
               success:(void (^)(void))success
               failure:(void (^)(NSError *error))failure;

/**
 Upload file to specific directory on remote server.
 
 @param localPath Path of local file to upload.
 @param toPath Remote path where file will be uploaded to.
 @param progress Calls after data has been sent to remote server.
        Return NO to cancel the operation.
 @return YES on success. NO on failure.
 */
- (BOOL)uploadFile:(NSString *)localPath to:(NSString *)remotePath
          progress:(BOOL (^)(NSUInteger sent, NSUInteger totalBytes))progress;

/**
 Refer to uploadFile:to:progress:
 
 This adds the ability to perform the operation asynchronously.
 
 @param localPath Path of local file to upload.
 @param toPath Remote path where file will be uploaded to.
 @param progress Calls after data has been sent to remote server.
        Return NO to cancel the operation.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 */
- (void)uploadFile:(NSString *)localPath to:(NSString *)remotePath
          progress:(BOOL (^)(NSUInteger sent, NSUInteger totalBytes))progress
           success:(void (^)(void))success
           failure:(void (^)(NSError *error))failure;

/**
 Create directory at the specified path on the remote server.
 
 @param remotePath Path to create remote directory.
 @return YES on success. NO on failure.
 */
- (BOOL)createDirectoryAtPath:(NSString *)remotePath;

/**
 Refer to createDirectoryAtPath:
 
 @param remotePath Path to create remote directory.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 */
- (void)createDirectoryAtPath:(NSString *)remotePath
                      success:(void (^)(void))success
                      failure:(void (^)(NSError *error))failure;

/**
 Create remote directory within the handle's location.
 
 @param directoryName Name of directory to create on remote server.
 @param remotePath Path to remote directory where file should be created.
 @return YES on success. NO on failure.
 */
- (BOOL)createDirectoryAtHandle:(FTPHandle *)handle;

/**
 Refer to createDirectoryAtHandle:
 
 This adds the ability to perform the operation asynchronously.
 
 @param directoryName Name of directory to create on remote server.
 @param remotePath Path to remote directory where file should be created.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 */
- (void)createDirectoryAtHandle:(FTPHandle *)handle
                        success:(void (^)(void))success
                        failure:(void (^)(NSError *error))failure;

/**
 Delete directory at specified remote path.
 
 @param remotePath The path of the remote directory to delete.
 @return YES on success. NO on failure.
 */
- (BOOL)deleteDirectoryAtPath:(NSString *)remotePath;

/**
 Refer to deleteDirectoryAtPath:
 
 This adds the ability to perform the operation asynchronously.
 
 @param remotePath The path of the remote directory to delete.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 */
- (void)deleteDirectoryAtPath:(NSString *)remotePath
                      success:(void (^)(void))success
                      failure:(void (^)(NSError *error))failure;

/**
 Delete a file at a specified remote path.
 
 @param remotePath The path to the remote resource to delete.
 @return YES on success. NO on failure.
 */
- (BOOL)deleteFileAtPath:(NSString *)remotePath;

/**
 Refer to deleteFileAtPath:
 
 This adds the ability to perform the operation asynchronously.
 
 @param remotePath The path to the remote resource to delete.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 @return FTPRequest The request instance.
 */
- (void)deleteFileAtPath:(NSString *)remotePath
                 success:(void (^)(void))success
                 failure:(void (^)(NSError *error))failure;

/**
 Delete a remote handle from the server.
 
 @param handle The remote handle to delete.
 @return YES on success. NO on failure.
 */
- (BOOL)deleteHandle:(FTPHandle *)handle;

/**
 Refer to deleteHandle:
 
 This adds the ability to perform the operation asynchronously.
 
 @param handle The remote handle to delete.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 @return FTPRequest The request instance.
 */
- (void)deleteHandle:(FTPHandle *)handle
             success:(void (^)(void))success
             failure:(void (^)(NSError *error))failure;

/**
 Change file mode of a remote file or directory.
 
 @param remotePath Full path to remote resource.
 @param mode File mode to change to.
 @return YES on success. NO on failure.
 */
- (BOOL)chmodPath:(NSString *)remotePath toMode:(int)mode;

/**
 Refer to chmodPath:toMode:
 
 This adds the ability to perform the operation asynchronously.
 
 @param remotePath Full path to remote resource.
 @param mode File mode to change to.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 */
- (void)chmodPath:(NSString *)remotePath toMode:(int)mode
          success:(void (^)(void))success
          failure:(void (^)(NSError *error))failure;

/**
 Change file mode of a remote file or directory.
 
 @param handle The remote handle.
 @param mode File mode to change to.
 @return YES on success. NO on failure.
 */
- (BOOL)chmodHandle:(FTPHandle *)handle toMode:(int)mode;

/**
 Refer to chmodHandle:toMode:
 
 This adds the ability to perform the operation asynchronously.
 
 @param handle The remote handle to change mode on.
 @param mode File mode to change to.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 */
- (void)chmodHandle:(FTPHandle *)handle toMode:(int)mode
            success:(void (^)(void))success
            failure:(void (^)(NSError *error))failure;

/**
 Rename a remote path to something else. This method can be used to move a
 file to a different directory.
 
 @param sourcePath Source path to rename.
 @param destPath Destination of renamed file.
 */
- (BOOL)renamePath:(NSString *)sourcePath to:(NSString *)destPath;

/**
 Refer to renamePath:to:
 
 This adds the ability to perform the operation asynchronously.
 
 @param sourcePath Source path to rename.
 @param destPath Destination of renamed file.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 */
- (void)renamePath:(NSString *)sourcePath to:(NSString *)destPath
           success:(void (^)(void))success
           failure:(void (^)(NSError *error))failure;

/**
 Copy a remote path to another location.
 
 @param sourcePath Source path to copy.
 @param destPath Destination of copied file.
 */
- (BOOL)copyPath:(NSString *)sourcePath to:(NSString *)destPath;

/**
 Refer to copyPath:to:
 
 This adds the ability to perform the operation asynchronously.
 
 @param sourcePath Source path to copy.
 @param destPath Destination of copied file.
 @param success Method called when process succeeds.
 @param failure Method called when process fails.
 */
- (void)copyPath:(NSString *)sourcePath to:(NSString *)destPath
         success:(void (^)(void))success
         failure:(void (^)(NSError *error))failure;

/**
 Returns the last modification date of remotePath. This will NOT work with
 directories, as the RFC spec does not require it.
 
 @param remotePath Path to get modified date for
 @return Date the remote path was last modified
 */
- (NSDate *)lastModifiedAtPath:(NSString *)remotePath;

/**
 Check if a remote directory exists.
 
 Please note that this internally calls [self changeDirectoryToPath:] and does
 _not_ change back to the previous directory!
 
 @param remotePath Directory to check
 @return YES if the directory exists. NO, otherwise
 */
- (BOOL)directoryExistsAtPath:(NSString *)remotePath;

/**
 Change the working directory to remotePath.
 
 @param remotePath Remote directory path to make current directory
 @return YES if the directory was successfully changed.
 */
- (BOOL)changeDirectoryToPath:(NSString *)remotePath;

@end