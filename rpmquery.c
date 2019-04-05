#include <stdio.h>
#include <rpm/rpmlib.h>
#include <rpm/rpmts.h>
#include <rpm/header.h>

void printtagdata(const rpmtd atagdata ) {
  char* datastr = NULL;
  if( !atagdata ) {
    printf("NULL POINTER\n");
    return;
  }

  const char* tagname = (atagdata->tag) ? 
    rpmTagGetName(atagdata->tag) : "!NULLTAG!";

  if (atagdata->data) {
    datastr = rpmtdFormat(atagdata, RPMTD_FORMAT_STRING, "SOME_ERROR");
  }
  else {
    datastr = strdup("!NULLDATA!");    
  }
  printf("%s : ", tagname);
  if( atagdata->count > 1 ) {
    printf("ARRAY_OF_SIZE_%u\n", atagdata->count);
  }
  else {
    printf("%s\n", datastr);
  }
  free(datastr); datastr = 0;
  return;
}


int main(int argc, char** argv) {
  int result = 0;
  if( argc != 2 ) {
    printf("Usage:\n\trpmquery [ignored-options] <file.rpm>\n");
    exit(1);
  }

  const char* fnamein = argv[argc-1];
  FD_t fdin = Fopen(fnamein, "r.ufdio");
  if ((!fdin) || Ferror(fdin)) {
    fprintf(stderr, "Failed to open \"%s\": %s\n", fnamein, Fstrerror(fdin));
    if (fdin) {
      Fclose(fdin);
    }
    exit(1);
  }

  rpmts ts = rpmtsCreate();
  rpmVSFlags vsflags = 0;
  vsflags |= _RPMVSF_NODIGESTS;
  vsflags |= _RPMVSF_NOSIGNATURES;
  vsflags |= RPMVSF_NOHDRCHK;
  (void) rpmtsSetVSFlags(ts, vsflags);
 
  rpmRC rc = 0;
  Header rpmhdr = {0};
  rc = rpmReadPackageFile(ts, fdin, fnamein, &rpmhdr);
  if (rc != RPMRC_OK) {
    fprintf(stderr, "Could not read package header for %s\n", fnamein);
    Fclose(fdin);
    exit(1);
  }

  HeaderIterator hi = headerInitIterator(rpmhdr);
  struct rpmtd_s curdata = {0};
  rpmtd cptr = &curdata;

  // iterate over header entries printing each one
  while (headerNext(hi, cptr)) {    
    printtagdata(cptr);
    rpmtdFreeData(cptr);
  }

  // Clean up
  headerFreeIterator(hi);
  headerFree(rpmhdr);
  rpmtsFree(ts);

  Fclose(fdin);

  return result;
}
